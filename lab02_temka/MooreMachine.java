import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MooreMachine {
    private Map<MooreState, Map<MooreSignal, MooreState>> matrix;
    private MooreState currentState;

    public MooreMachine(List<MooreStep> additionalSteps) {
        this.matrix = getDefaultMatrix();
        this.currentState = MooreState.START_STATE;

        // Update the matrix with given steps
        additionalSteps.stream().forEach(
            step -> matrix.get(step.from).replace(step.signal, step.to)
        );
    }

    private static Map<MooreState, Map<MooreSignal, MooreState>> getDefaultMatrix() {
        Map<MooreState, Map<MooreSignal, MooreState>> matrix =
            new HashMap<>();
        for (MooreState state : MooreState.values()) {
            Map<MooreSignal, MooreState> steps = new HashMap<>();
            for (MooreSignal signal : MooreSignal.values()) {
                steps.put(signal, state.getNext());
            }

            matrix.put(state, steps);
        }

        return matrix;
    }

    public void processStringSignal(String string) {
        processSignal(MooreSignal.valueOf(string.toUpperCase()));
    }

    public void processStringsSignals(String[] signals) {
        for (String signal : signals) {
            processStringSignal(signal);
        }
    }

    public void processSignal(MooreSignal signal) {
        System.out.print("Signal: " + signal + " ; ");

        if (currentState.equals(MooreState.END_STATE)) {
            System.out.println("Reached the End State!");
        } else {
            final MooreState newState
                = matrix.get(currentState).get(signal);

            System.out.println("From: " + currentState.index +
                    " to " + newState.index);

            this.currentState = newState;
        }
    }

    public void processSignals(List<MooreSignal> signals) {
        signals.stream().forEach(signal -> processSignal(signal));
    }
}
