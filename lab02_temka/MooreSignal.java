public enum MooreSignal {
    DLM("dlm"),
    CFR("cfr"),
    LTR("ltr");

    MooreSignal(String string) {
        this.string = string;
    }

    @Override
    public String toString() {
        return this.string;
    }

    private final String string;
}
