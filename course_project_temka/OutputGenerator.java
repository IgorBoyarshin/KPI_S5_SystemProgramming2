import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class OutputGenerator {

    private final StringBuilder code;
    private final Map<String, Type> variables;

    public OutputGenerator(StringBuilder code, Map<String, Type> variables) {
        this.code = code;
        this.variables = variables;
    }

    private StringBuilder generateDeclarationCode(Map<String, Type> variables) {
        StringBuilder declaration = new StringBuilder();
        declaration.append(".data\n");
        for (Map.Entry<String, Type> var : variables.entrySet()) {
            declaration.append("\t" + var.getKey() + " ");
            switch (var.getValue()) {
                case Bool:
                    declaration.append("DB");
                    break;
                case Int:
                case PointerBool:
                case PointerInt:
                case PointerFloat:
                    declaration.append("DD");
                    break;
                case Float:
                    declaration.append("DQ");
                    break;
            }
            declaration.append(" ?\n");
        }

        return declaration;
    }

    public String generateCode() {
        StringBuilder result = new StringBuilder();

        result.append(".586\n");
        result.append("\n");
        result.append(generateDeclarationCode(variables));
        result.append("\n");
        result.append(".code\n");
        result.append("main proc\n");
        result.append("\tpush EBP\n");
        result.append("\tmov EBP, ESP\n");

        result.append(code);

        result.append("\tmov ESP, EBP\n");
        result.append("\tpop EBP\n");
        result.append("\tret\n");
        result.append("main endp\n");

        return result.toString();
    }
}
