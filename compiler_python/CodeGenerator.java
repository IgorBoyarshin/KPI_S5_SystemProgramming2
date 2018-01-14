// package CourseWord;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;

public class CodeGenerator {
    private StringBuilder code;

    private List<Node> nodes;
    private Map<String, Type> variablesTable;

    public CodeGenerator(List<Node> nodes, Map<String, Type> variablesTable) {
        this.code = new StringBuilder();
        this.nodes = nodes;
        this.variablesTable = variablesTable;
    }

    public String generate() {
        code.append(".data\n");
        for (Map.Entry<String, Type> entry : variablesTable.entrySet()) {
            final Type variableType = entry.getValue();
            if (!(variableType == Type.BOOLEAN || variableType == Type.NUM)) {
                continue;
            }
            final String variableName = entry.getKey();
            code.append("\t")
                .append(variableName)
                .append(" ")
                .append(variableType == Type.BOOLEAN ? "db" : "dd")
                .append(" 0")
                .append("\n");
        }
        code.append("\n");

        code.append(".code\n");

        List<Node> intoMain = new ArrayList<>();
        boolean inFunction = false;
        for (Node node : nodes) {
            switch (node.name) {
                case FUNCTION_NEW:
                    inFunction = true;

                    final FunctionNew nodeFuncNew = (FunctionNew) node;
                    code.append(nodeFuncNew.name)
                        .append(" proc\n");
                    code.append("\tpush ebp").append("\n");
                    code.append("\tmov ebp, esp").append("\n");
                    code.append("\n");
                    for (int i = 0; i < nodeFuncNew.listOfArguments.size(); i++) {
                        final String name = nodeFuncNew.listOfArguments.get(i).name;
                        final int stackIndex =
                            8 + (nodeFuncNew.listOfArguments.size() - i - 1) * 4;
                        code.append("\tmov eax, dword ptr[ebp + ")
                            .append(stackIndex)
                            .append("]\n");
                        code.append("\tmov ")
                            .append(name)
                            .append(", eax\n");
                    }
                    code.append("\n");
                    break;
                case FUNCTION_END:
                    inFunction = false;

                    final FunctionEnd nodeFuncEnd = (FunctionEnd) node;
                    code.append("\n");
                    code.append(nodeFuncEnd.name + "End:\n");
                    code.append("\tmov esp, ebp").append("\n")
                        .append("\tpop ebp").append("\n")
                        .append("\tret ").append((4*nodeFuncEnd.amountOfArguments)).append("\n");
                    code.append(nodeFuncEnd.name)
                        .append(" endp\n");
                    break;
                case STATEMENT:
                    if (inFunction) {
                        code.append(((Statement)node).code);
                    } else {
                        intoMain.add(node);
                    }
                    break;
            }
        }
        code.append("\n");

        code.append("\n");
        code.append("main proc").append("\n");
        code.append("\tpush ebp").append("\n");
        code.append("\tmov ebp, esp").append("\n");
        code.append("\n");
        for (Node node : intoMain) {
            code.append(((Statement)node).code);
        }
        code.append("\tmov esp, ebp").append("\n");
        code.append("\tpop ebp").append("\n");
        code.append("\tret").append("\n");
        code.append("main endp").append("\n");

        return code.toString();
    }
}
