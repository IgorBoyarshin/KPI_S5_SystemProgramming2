/*
 * System Programming
 * "Modular programming in basic program design strategy and its usage in
 * programs for table creation"
 *
 * Author: Artem Berezinec, #5205, IO-52, FIOT
 * Date: 08.10.17
 */

class Lab1 {
    public static void main(String[] args) {
        System.out.println();

        Table table = new Table();
        table.directInsert(new Row("key1", (short)2, Field.FieldValue1), (char)0);
        table.directInsert(new Row("key2", (short)3, Field.FieldValue3), (char)1);
        table.directUpdate(new Row("key3", (short)4, Field.FieldValue2), (char)1);

        System.out.println(table);

        table.linearInsert(new Row("key4", (short)2, Field.FieldValue1));
        table.linearDelete(new Row("key4", (short)2, Field.FieldValue1));
        table.linearInsert(new Row("akey5", (short)4, Field.FieldValue2));

        table.sort();
        System.out.println(table);

        table.binaryInsert(new Row("temka", (short)0, Field.FieldValue2));
        table.binaryDelete(new Row("mashka", (short)0, Field.FieldValue3));

        System.out.println(table);

        table.linearPack();

        System.out.println(table);

        Table table2 = new Table();
        // table2.linearInsert(new Row("abcde", (short)3, Field.FieldValue1));
        table2.linearInsert(new Row("BaEd", (short)3, Field.FieldValue2));
        table2.linearInsert(new Row("DDCCAA", (short)3, Field.FieldValue3));
        table2.linearInsert(new Row("DDeCCAA", (short)3, Field.FieldValue1));

        System.out.println(table2);

        System.out.println("Best matches:");
        for (Row row : table2.selectRowsWithBestMatch(new Key("dEbAuC", (short)3))) {
            System.out.println(row);
        }
    }
}
