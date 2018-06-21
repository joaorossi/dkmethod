namespace dkm
{

class Model
{
public:
    Model (double sampleRate = 44100.0);

    /** Add a component to the current Net List

        For safely adding components, create them using the ComponentFactory;
    */
    void addComponent (Component component);

    /** Process model.

        The AudioBlock should contain <numOfInputs> channels. In the
        same order the input components were added.
    */
    void process (dsp::AudioBlock<double>);

    void calculateMatrixes();

private:
    // System Matrixes
    arma::mat A, B, C, D, E, F, G, H, K;

    // Input
    arma::mat U;

    // State
    arma::mat X;

    // Unkown voltages
    arma::mat V;

    /** Model non-linearity

        Whole model non-linear function. Contatenates the non-linear function of all components.
        Takes a voltage vector v as input.
        Returns the current vector i, and the Jacobian matrix J.
    */
    void modelNonlinearity (const arma::mat& v, arma::mat& i, arma::mat& J);

    /** Non-linear solver

        Solves model non-linearity.
        Takes the p and k matrixes and input, and return a matrix where the first
        columns is the voltages vector and the second columns the currents vector.
    */
    void solveNonlinearFunc (const arma::mat& p, const arma::mat& k, arma::mat& i, arma::mat& v);
    const int maxIter = 10;
    const double eps = 0.05;

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

    // Model non-linearities helpers
    arma::mat it, Jt, e, J;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Model)
};

} // namespce DK
} // namespce royale
