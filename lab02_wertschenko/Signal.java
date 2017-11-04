public enum Signal {
    DLM("dlm"),
    CFR("cfr"),
    LTR("ltr");

    private final String name;

    Signal(String name) {
        this.name = name;
    }

    public static Signal getFromString(String name) {
        name = name.toLowerCase();
        for (Signal signal : DLM.values()) {
            final String signalName = signal.toString();
            if (signalName.equals(name)) {
                return signal;
            }
        }

        return null;
    }

    @Override
    public String toString() {
        return this.name;
    }
}
