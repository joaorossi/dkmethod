namespace dkm
{

class Model
{
public:
    Model (double initialSampleRate = 48000.0);

    /** Add a component to the current Net List

        For safely adding components, use the ComponentFactory.
        You must have added the complete circuit before calling any
        of the methods below.
    */
    void addComponent (Component component);

    /** Process model.

        The AudioBlock should contain <numOfInputs> channels. In the
        same order the input components were added.

        For now, only double precision is supported, when a more robust
        non-linear solver is implemented float should be considered.
    */
    void process (const dsp::ProcessContextReplacing<double>&);

    /** Prepera model for playing

        Update matrixes for new sample rate and calculate system steady state.
        Should be called before starting a new audio stream.
    */
    void prepare (const dsp::ProcessSpec&);

    /** Reset model internal state.

        Sets the state matrix X and non-linear voltages vector V to zeros.
    */
    void reset();

private:
    // System Matrixes
    arma::mat A, B, C, D, E, F, G, H, K;

    // Input
    arma::mat U;

    // State
    arma::mat X;

    // Non-linear voltages
    arma::mat V;

    /** Model non-linearity

        Whole model non-linear function. Contatenates the non-linear function of all components.
        Takes a voltage vector v as input.
        Returns the current vector i, and the Jacobian matrix J.
    */
    void modelNonlinearity (const arma::mat& v, arma::mat& i, arma::mat& J);

    /** Non-linear solver

        Solves model non-linearity.
        Takes the p and k matrixes as inputs, and return a matrix where the first
        columns is the voltages vector and the second columns the currents vector.
    */
    void solveNonlinearFunc (const arma::mat& p, const arma::mat& k, arma::mat& i, arma::mat& v);
    const int maxIter = 10;
    const double eps = 0.05;

    void calculateMatrixes();

    double sampleRate;
    double samplePeriod;

    Array<Component> input, output;
    Array<Component> resistor;
    Array<Component> capacitor;
    Array<Component> inductor;
    Array<Component> opa;
    Array<Component> potentiometer;
    Array<Component> nonlinear;

    Array<int> nodes;

    // Model non-linearities helpers (avoids memory allocation in processing)
    arma::mat it, Jt, e, J, iComp1, jComp1, iComp2, jComp2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Model)
};

} // namespce dkm
