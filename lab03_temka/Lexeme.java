public class Lexeme {
    private final LexemeType type;
    private final String value;

    public Lexeme(String value) {
        this.type = LexemeType.Unknown;
        this.value = value;
    }

    public Lexeme(LexemeType type, String value) {
        this.type = type;
        this.value = value;
    }

    @Override
    public String toString() {
        return value + " <-> " + type;
    }
}
