public enum Type {
    Int, Float, Bool,
    PointerInt, PointerFloat, PointerBool;

    @Override
    public String toString() {
        switch (this) {
            case Int:
                return "int";
            case Float:
                return "float";
            case Bool:
                return "bool";
            case PointerInt:
                return "int*";
            case PointerFloat:
                return "float*";
            case PointerBool:
                return "bool*";
        }

        return "";
    }
}
