/*
 * System Programming
 * Lab03
 *
 * Artem Berezinec
 * 04.11.17
 */


public class Lab3 {
    public static void main(String[] args) {
        LexicalAnalyzer la = new LexicalAnalyzer();
        final String input = "b23 += 7a8d[--n];";
        System.out.println(input);
        la.analyze(input);
        la.print();
    }
}
