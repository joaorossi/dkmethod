namespace dkm
{

Model::Model (double sr) :
    sampleRate (sr),
    samplePeriod (1.0 / sampleRate)
{
}

void Model::addComponent (Component component)
{
    if (component.type == ComponentType::input)
        input.add (component);
    if (component.type == ComponentType::output)
        output.add (component);
    if (component.type == ComponentType::resistor)
        resistor.add (component);
    if (component.type == ComponentType::capacitor)
        capacitor.add (component);
    if (component.type == ComponentType::inductor)
        inductor.add (component);
    if (component.type == ComponentType::opa)
        opa.add (component);
    if (component.type == ComponentType::potentiometer)
        potentiometer.add (component);
    if (component.type == ComponentType::diode
        || component.type == ComponentType::transistor
        || component.type == ComponentType::triode)
        nonlinear.add (component);

    for (auto componentNode : component.nodes)
        nodes.addIfNotAlreadyThere (componentNode);
}

void Model::process (dsp::AudioBlock<double> block)
{
    auto samples = block.getNumSamples();

    // Nonlinear currents
    arma::mat I;

    arma::mat p, out;

    for (int n = 0; n < samples; ++n)
    {
        for (int i = 0; i < input.size(); ++i)
            U (i, 0) = block.getSample (i, n);

        p = (G * X) + (H * U);
        solveNonlinearFunc (p, K, I, V);

        out = (D * X) + (E * U) + (F * I);
        X = (A * X) + (B * U) + (C * I);

        for (int i = 0; i < output.size(); ++i)
            block.setSample (i, n, out (i, 0));
    }
}

void Model::solveNonlinearFunc (const arma::mat& p, const arma::mat& k, arma::mat& i, arma::mat& v)
{
    // Newton-Raphson
    int iter = maxIter;

    arma::mat v0 = v;
    arma::mat v1 = v0 + 2.0 * eps;

    arma::mat eye = arma::eye (v1.n_rows, v1.n_rows);

    while (iter > 0 && arma::any (arma::vectorise (arma::abs (v1 - v0)) > eps))
    {
        v0 = v1;
        modelNonlinearity (v1, it, Jt);
        e = p + (k * it) - v1;
        J = (k * Jt) - eye;
        v1 = v0 - arma::solve (J, e);
        --iter;
    }
    v = v1;
    modelNonlinearity (v, i, J);
}

void Model::modelNonlinearity (const arma::mat& v, arma::mat& i, arma::mat& j)
{
    for (int nl = 0; nl < nonlinear.size();)
    {
        auto& comp = nonlinear.getReference (nl);
        auto ports = comp.numOfPorts;

        arma::mat iComp, jComp;
        iComp.set_size (ports, 1);
        jComp.set_size (ports, ports);
        
        if (ports == 1)
        {
            comp.model (v.row (nl), iComp, jComp);
            i.submat (nl, 0, nl, 0) = iComp;
            j.submat (nl, nl, nl, nl) = jComp;
        }
        if (ports == 2)
        {
            comp.model (v.rows (nl, nl + 1), iComp, jComp);
            i.submat (nl, 0, nl + 1, 0) = iComp;
            j.submat (nl, nl, nl + 1, nl + 1) = jComp;
        }

        nl += ports;
    }
}

void Model::calculateMatrixes()
{

    arma::mat Nr = arma::zeros<arma::mat> (resistor.size(), nodes.size());
    arma::mat Gr = arma::zeros<arma::mat> (resistor.size(), resistor.size());

    arma::mat Nx = arma::zeros<arma::mat> (capacitor.size() + inductor.size(), nodes.size());
    arma::mat Gx = arma::zeros<arma::mat> (capacitor.size() + inductor.size(), capacitor.size() + inductor.size());
    arma::mat Z = arma::zeros<arma::mat> (capacitor.size() + inductor.size(), capacitor.size() + inductor.size());

    arma::mat Nu = arma::zeros<arma::mat> (input.size(), nodes.size());
    arma::mat No = arma::zeros<arma::mat> (output.size(), nodes.size());

    arma::mat Nn;
    {
        int nonlinearPorts = 0;
        for (auto nl : nonlinear)
            nonlinearPorts += nl.numOfPorts;
        Nn = arma::zeros<arma::mat> (nonlinearPorts, nodes.size());

        Jt = arma::zeros<arma::mat> (nonlinearPorts, nonlinearPorts);
        it = arma::zeros<arma::mat> (nonlinearPorts, 1);
    }

    //arma::mat Nv = arma::zeros<arma::mat> (potentiometer.size(), nodes.size()); TODO!!

    arma::mat Nopai = arma::zeros<arma::mat> (opa.size(), nodes.size());
    arma::mat Nopao = arma::zeros<arma::mat> (opa.size(), nodes.size());

    // Input size
    U = arma::zeros<arma::mat> (input.size(), 1);

    // State size
    X = arma::zeros<arma::mat> (capacitor.size() + inductor.size(), 1);

    // Nonlinear voltages
    V = arma::zeros<arma::mat> (Nn.n_rows, 1);

    for (int i = 0; i < input.size(); ++i)
    {
        Nu (i, input.getReference(i).nodes[0]) = 1.0;
        Nu (i, input.getReference(i).nodes[1]) = -1.0;
    }

    for (int i = 0; i < output.size(); ++i)
    {
        No (i, output.getReference(i).nodes[0]) = 1.0;
        No (i, output.getReference(i).nodes[1]) = -1.0;
    }

    // TODO - Potentiometer

    for (int i = 0; i < opa.size(); ++i)
    {
        Nopai (i, opa.getReference(i).nodes[0]) = 1.0;
        Nopai (i, opa.getReference(i).nodes[1]) = -1.0;
        Nopao (i, opa.getReference(i).nodes[2]) = 1.0;
    }

    for (int i = 0; i < resistor.size(); ++i)
    {
        Gr (i, i) = 1.0 / resistor.getReference(i).value;
        Nr (i, resistor.getReference(i).nodes[0]) = 1.0;
        Nr (i, resistor.getReference(i).nodes[1]) = -1.0;
    }

    for (int i = 0; i < capacitor.size() + inductor.size(); ++i)
    {
        if (i < capacitor.size())
        {
            Z (i, i) = 1.0;
            Gx (i, i) = 2.0 * capacitor.getReference(i).value / samplePeriod;
            Nx (i, capacitor.getReference(i).nodes[0]) = 1.0;
            Nx (i, capacitor.getReference(i).nodes[1]) = -1.0;
        }
        else
        {
            Z (i, i) = -1.0;
            Gx (i, i) = samplePeriod / (2.0 * inductor.getReference(i - capacitor.size()).value);
            Nx (i, inductor.getReference(i - capacitor.size()).nodes[0]) = 1.0;
            Nx (i, inductor.getReference(i - capacitor.size()).nodes[1]) = -1.0;
        }
    }

    for (int i = 0; i < nonlinear.size(); ++i)
    {
        Nn (i, nonlinear.getReference(i).nodes[0]) = 1.0;
        Nn (i, nonlinear.getReference(i).nodes[1]) = -1.0;

        if (nonlinear.getReference(i).numOfPorts == 2)
        {
            Nn (i + 1, nonlinear.getReference(i).nodes[2]) = 1.0;
            Nn (i + 1, nonlinear.getReference(i).nodes[3]) = -1.0;
        }
    }

    Nr.shed_col (0);
    Nx.shed_col (0);
    Nn.shed_col (0);
//    Nv.shed_col (0);
    Nu.shed_col (0);
    No.shed_col (0);
    Nopao.shed_col (0);
    Nopai.shed_col (0);

    Nr.print ("Nr:");
    Gr.print ("Gr:");
    Nx.print ("Nx:");
    Gx.print ("Gx:");
    Z.print ("Z:");
    Nu.print ("Nu:");
    No.print ("No:");
    Nn.print ("Nn:");
//    Nv.print ("Nv:");
    Nopai.print ("Nopao:");
    Nopao.print ("Nopai:");
    U.print ("U:");

    ScopedNoDenormals noDenomals;

    auto numVoltageSources = Nu.n_rows;
    arma::mat S;
    {
        arma::mat tmp1 = arma::join_horiz ((Nr.t() * Gr * Nr) + (Nx.t() * Gx * Nx), Nu.t());
        arma::mat tmp2 = arma::join_horiz (Nu, arma::zeros<arma::mat> (numVoltageSources, numVoltageSources));
        S = arma::join_vert (tmp1, tmp2);
        S.print ("S:");
    }

    // Extend conductance matrix for OPAs
    if (opa.size() > 0)
    {
        Nopao = arma::join_horiz (Nopao, arma::zeros<arma::mat> (Nopao.n_rows, numVoltageSources));
        Nopai = arma::join_horiz (Nopai, arma::zeros<arma::mat> (Nopai.n_rows, numVoltageSources));

        arma::mat tmp1 = arma::join_horiz (S, Nopao.t());
        arma::mat tmp2 = arma::join_horiz (Nopao + (Nopai * 1000000.0),
                                           arma::zeros<arma::mat> (Nopao.n_rows, Nopao.n_rows));

        S = arma::join_vert (tmp1, tmp2);
        S.print ("S with OPAs:");
    }

    arma::mat Nxp = arma::join_horiz (Nx, arma::zeros<arma::mat> (Nx.n_rows, numVoltageSources + opa.size()));
    arma::mat Nnp = arma::join_horiz (Nn, arma::zeros<arma::mat> (Nn.n_rows, numVoltageSources + opa.size()));
    arma::mat Nop = arma::join_horiz (No, arma::zeros<arma::mat> (No.n_rows, numVoltageSources + opa.size()));
    arma::mat Nup = arma::join_horiz (Nu, arma::zeros<arma::mat> (Nu.n_rows, numVoltageSources));
    arma::mat Nup2 = arma::join_vert (arma::zeros<arma::mat> (Nu.n_cols, numVoltageSources),
                                      arma::eye<arma::mat> (numVoltageSources, numVoltageSources));

    if (opa.size() > 0)
        Nup2 = arma::join_vert (Nup2, arma::zeros<arma::mat> (opa.size(), numVoltageSources));

    arma::mat Si = S.i();
    Si.print ("Si:");

    // Calculate system matrixes
    arma::mat NxpT = Nxp.t();
    arma::mat NnpT = Nnp.t();

    A = ((Z * Gx * Nxp * Si * NxpT) * 2.0) - Z;
    A.print ("A:");

    B = (Z * Gx * Nxp * Si * Nup2) * 2.0;
    B.print ("B:");

    C = (Z * Gx * Nxp * Si * NnpT) * 2.0;
    C.print ("C:");

    D = Nop * Si * NxpT;
    D.print ("D:");

    E = Nop * Si * Nup2;
    E.print ("E:");

    F = Nop * Si * NnpT;
    F.print ("F:");

    G = Nnp * Si * NxpT;
    G.print ("G:");

    H = Nnp * Si * Nup2;
    H.print ("H:");

    K = Nnp * Si * NnpT;
    K.print ("K:");
}

} // namespce dkm
