public enum MooreState {
    MooreState1(1),
    MooreState2(2),
    MooreState3(3),
    MooreState4(4),
    MooreState5(5),
    MooreState6(6),
    MooreState7(7),
    MooreState8(8);

    MooreState(int index) {
        this.index = index;
    }

    public MooreState getNext() {
        if (index == 8) {
            return this;
        }

        return MooreState.valueOf("MooreState" + (index + 1));
    }

    public final int index;
    public static final MooreState START_STATE = MooreState1;
    public static final MooreState END_STATE = MooreState8;
}
