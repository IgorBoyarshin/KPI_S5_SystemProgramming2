public enum Type {
    INTEGER,
    REAL,
    BOOLEAN,
    POINTER_INTEGER,
    POINTER_REAL,
    POINTER_BOOLEAN;

    @Override
    public String toString() {
        switch (this) {
            case INTEGER:
                return "integer";
            case REAL:
                return "real";
            case BOOLEAN:
                return "boolean";
            case POINTER_INTEGER:
                return "integer^";
            case POINTER_REAL:
                return "real^";
            case POINTER_BOOLEAN:
                return "boolean^";
        }

        return "";
    }
}
