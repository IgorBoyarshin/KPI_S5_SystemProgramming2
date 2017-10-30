/*
 * System Programming - 2
 * "Tables and operation on them"
 *
 * Author: Maksim Wertschenko
 */

class Lab1 {
    public static void main(String[] args) {
        Table table = new Table();
        table.insertDirect(new TableRow("monkey", 0, 0.0), (char)0);
        table.updateDirect(new TableRow("elephant", 1, 1.0), (char)0);
        table.insertLinear(new TableRow("dinosaur", 5, 5.0));
        table.deleteLinear(new TableRow("dinosaur", 5, 5.0));

        table.insertBinary(new TableRow("aBcd12", 2, 2.0));
        table.insertBinary(new TableRow("AbcD34", 3, 3.0));
        table.insertBinary(new TableRow("dddcc", 4, 4.0));
        table.insertBinary(new TableRow("aaccDDee", 5, 5.0));

        table.pack();

        System.out.println(table);

        final String stringToCompareAgainst = "dd89abc";
        System.out.println("Best fit:");
        for (TableRow row :
                table.getAllRowsThatFitBest(new RowKey(stringToCompareAgainst, 8))) {
            System.out.println(row);
        }
    }
}
