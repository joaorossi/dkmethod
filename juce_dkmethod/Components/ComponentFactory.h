namespace dkm
{

struct ComponentFactory
{
    ComponentFactory() = delete;
    ~ComponentFactory() = delete;

    static Component makeInput (const String& name,
                                int nodePositive, int nodeNegative);

    static Component makeOutput (const String& name,
                                 int nodePositive, int nodeNegative);

    static Component makePotentiometer (double maxResistance, const String& name,
                                        int nodePositive, int nodeNegative, int nodeTap);

    static Component makeOPA (double gain, const String& name,
                              int nodeInPositive, int nodeInNegative, int nodeOut);

    static Component makeResistor (double resistance, const String& name,
                                   int nodePositive, int nodeNegative);

    static Component makeCapacitor (double capacitance, const String& name,
                                    int nodePositive, int nodeNegative);

    static Component makeInductor (double inductance, const String& name,
                                   int nodePositive, int nodeNegative);

    static Component makeDiode (const String& name,
                                int nodeCathode, int nodeAnode);

    static Component makeTransistor (const String& name,
                                     int nodeBase, int nodeColector, int nodeEmiter);

    static Component makeTriode (const String& name,
                                 int nodeGrid, int nodePlate, int nodeCathode);

    JUCE_DECLARE_NON_COPYABLE (ComponentFactory)
};

} // namespace dkm
