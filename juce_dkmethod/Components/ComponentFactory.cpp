namespace dkm
{

Component ComponentFactory::makeInput (const String& name,
                                       int nodePositive, int nodeNegative)
{
    Component comp = { ComponentType::input, name, 0.0,
        { nodePositive, nodeNegative }, 1, false };
    return comp;
}

Component ComponentFactory::makeOutput (const String& name,
                                        int nodePositive, int nodeNegative)
{
    Component comp = { ComponentType::output, name, 0.0,
        { nodePositive, nodeNegative }, 1, false };
    return comp;
}

Component ComponentFactory::makePotentiometer (double maxResistance, const String& name,
                                               int nodePositive, int nodeNegative, int nodeTap)
{
    Component comp = { ComponentType::potentiometer, name, maxResistance,
        { nodePositive, nodeNegative, nodeTap }, 2, false };
    return comp;
}

Component ComponentFactory::makeOPA (double gain, const String& name,
                                     int nodeInPositive, int nodeInNegative, int nodeOut)
{
    Component comp = { ComponentType::input, name, gain,
        { nodeInPositive, nodeInNegative, nodeOut }, 1, false };
    return comp;
}

Component ComponentFactory::makeResistor (double resistance, const String& name,
                                          int nodePositive, int nodeNegative)
{
    Component comp = { ComponentType::resistor, name, resistance,
        { nodePositive, nodeNegative }, 1, false };
    return comp;
}


Component ComponentFactory::makeCapacitor (double capacitance, const String& name,
                                           int nodePositive, int nodeNegative)
{
    Component comp = { ComponentType::capacitor, name, capacitance,
        { nodePositive, nodeNegative }, 1, false };
    return comp;
}

Component ComponentFactory::makeInductor (double inductance, const String& name,
                                          int nodePositive, int nodeNegative)
{
    Component comp = { ComponentType::inductor, name, inductance,
        { nodePositive, nodeNegative }, 1, false };
    return comp;
}

Component ComponentFactory::makeTransistor (const String& name,
                                            int nodeBase, int nodeColector, int nodeEmiter)
{
    Component comp = { ComponentType::transistor, name, 0.0,
        { nodeBase, nodeEmiter, nodeColector, nodeEmiter }, 2, true,
        [] (const arma::mat& v, arma::mat& i, arma::mat& J)
        {
            i = v;
            J = arma::eye (v.n_rows, v.n_rows);
        } };
    return comp;
}

Component ComponentFactory::makeTriode (const String& name,
                                        int nodeGrid, int nodePlate, int nodeCathode)
{
    Component comp = { ComponentType::triode, name, 0.0,
        { nodeGrid, nodeCathode, nodePlate, nodeCathode }, 2, true,
        [] (const arma::mat& v, arma::mat& i, arma::mat& J)
        {
            double Vgk = v (0, 0);
            double Vpk = v (1, 0);

            double Gg = 0.000606;
            double xi = 1.354;
            double Cg = 13.9;
            double expg = std::exp (Cg * Vgk);
            double logg = std::log (1.0 + expg) / Cg;
            double Ig = -Gg * std::pow (logg, xi);
            double dIg_dVgk = -Gg * xi * std::pow (logg, xi - 1.0) * expg / (1.0 + expg);
            double dIg_dVpk = 0.0;

            double Gp = 0.00214;
            double gamma = 1.303;
            double Cp = 3.04;
            double mu = 100.8;
            double expp = std::exp (Cp * ((Vpk / mu) + Vgk));
            double logp = std::log (1.0 + expp) / Cp;
            double Ip = -Gp * std::pow (logp, gamma) - Ig;
            double dIp_dVgk = -Gp * gamma * std::pow (logp, gamma - 1.0) * expp / (1.0 + expp);
            double dIp_dVpk = dIp_dVgk / mu;

            i (0, 0) = Ig;
            i (1, 0) = Ip;

            J (0, 0) = dIg_dVgk;
            J (0, 1) = dIg_dVpk;
            J (1, 0) = dIp_dVgk;
            J (1, 1) = dIp_dVpk;
        } };
    return comp;
}

} // namespace dkm
