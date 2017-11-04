public enum State {
    State1(1),
    State2(2),
    State3(3),
    State4(4),
    State5(5),
    State6(6),
    State7(7);

    public final int index;
    public static final State startState = State1;
    public static final State endState = State7;

    State(int index) {
        this.index = index;
    }

    public State getNext() {
        if (index == 7) {
            return this;
        }

        return State.valueOf("State" + (index + 1));
    }
}
