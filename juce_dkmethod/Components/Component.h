namespace dkm
{

namespace ComponentType
{
#define DECL_TYPE(type) static const Identifier type (#type);

    DECL_TYPE (input)
    DECL_TYPE (output)
    DECL_TYPE (potentiometer)
    DECL_TYPE (opa)
    DECL_TYPE (resistor)
    DECL_TYPE (capacitor)
    DECL_TYPE (inductor)
    DECL_TYPE (diode)
    DECL_TYPE (transistor)
    DECL_TYPE (triode)

#undef DECL_TYPE
} // namespace ComponentType

// Non-linear function type. Should receive the voltages vector v
// and should write the currents vector i and the Jacobian mattrix J.
using NonlinearFunc = std::function<void(const arma::mat& v, arma::mat& i, arma::mat& J)>;

struct Component
{
    const Identifier type;
    const String name;
    const double value;

    const Array<int> nodes;

    const int numOfPorts;
    const bool isNonlinear;

    const NonlinearFunc model = nullptr;

    JUCE_LEAK_DETECTOR (Component)
};

} // namespace dkm

