public class MooreStep {
    public final MooreState from;
    public final MooreState to;
    public final MooreSignal signal;

    public MooreStep(MooreState from, MooreState to, MooreSignal signal) {
        this.from = from;
        this.to = to;
        this.signal = signal;
    }
}
