public class Lexeme {

    public final LexemeType type;
    public final String value;

    public Lexeme(LexemeType type, String value) {
        this.type = type;
        this.value = value;
    }

    @Override
    public String toString() {
        return "| Lexeme: " + value + " :: " + type.name();
    }
}
