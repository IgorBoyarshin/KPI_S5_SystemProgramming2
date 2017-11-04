import java.util.ArrayList;
import java.util.List;

/*
 * System Programming
 * "Moore Machine, nodes and trees"
 *
 * Author: Artem Berezinec, #5205, IO-52, FIOT
 * Date: 17.10.17
 */

public class Lab2 {
    public static void main(String[] args) {
        List<MooreStep> steps = new ArrayList<MooreStep>();
        steps.add(new MooreStep(MooreState.MooreState3, MooreState.MooreState8, MooreSignal.DLM));
        steps.add(new MooreStep(MooreState.MooreState5, MooreState.MooreState5, MooreSignal.LTR));

        MooreMachine moore = new MooreMachine(steps);

        moore.processStringsSignals("ltr dlm dlm cfr cfr".split(" "));
    }
}
