import java.util.ArrayList;

class main {

    private static String readNumber(String s, int i) {
        StringBuilder sb = new StringBuilder();
        while (i < s.length() && (Character.isDigit(s.charAt(i)) || s.charAt(i) == '.')) {
            sb.append(s.charAt(i));
            i++;
        }
        return sb.toString();
    }

    private static boolean isOperator(char c) {
        return c == '+'
                || c == '-'
                || c == '*' 
                || c == '/'
                || c == '%';
    }

    private static int getPrecedence(char c) {
        if ( c == '+' || c == '-' ) {
            return 1;
        } else if ( c == '*' || c == '/' || c == '%' ) {
            return 2;
        }
        return 0;
    }

    private static ArrayList<String> toRpn(String infix){
        int i = 0;
        ArrayList<String> output = new ArrayList<>();
        ArrayList<String> ops = new ArrayList<>();
        
        while ( i < infix.length() ) {
            char token = infix.charAt(i);
            if ( Character.isDigit(token)){
                String number = readNumber(infix, i);
                i += number.length();
                output.add(number);
                continue;
            }
            if ( isOperator(token) ) {
                char top = ops.isEmpty() ? ' ' : ops.get(ops.size() - 1).charAt(0);
                if ( top != ' ' && getPrecedence(top) >= getPrecedence(token) ) {
                    output.add(ops.remove(ops.size() - 1));
                }
                ops.add(String.valueOf(token));
                i += 1;
                continue;
            }
            if ( token == '('){
                ops.add(String.valueOf(token));
                i += 1;
                continue;
            }
            if (token == ')') {
                do {
                    String operator = ops.remove(ops.size() - 1);
                    if ( operator.charAt(0) == '(' ) {
                        break;
                    }
                    output.add(operator);
                } while ( !ops.isEmpty() );
                i += 1;
                continue;
            }
            i += 1;
        }
        while ( !ops.isEmpty() ) {
            output.add(ops.remove(ops.size() - 1));
        }
        return output;
    }

    private static String getValueOfExpression(String b, String a, char operator){
        
        switch ( operator ) {
            case '+':
                return String.valueOf(Double.parseDouble(b) + Double.parseDouble(a));
            case '-':
                return String.valueOf(Double.parseDouble(b) - Double.parseDouble(a));
            case '*':
                return String.valueOf(Double.parseDouble(b) * Double.parseDouble(a));
            case '/':
                return String.valueOf(Double.parseDouble(b) / Double.parseDouble(a));
            case '%':
                return String.valueOf(Double.parseDouble(b) % Double.parseDouble(a));
        }
        return "";
    }

    private static String calculate_rpn(ArrayList<String> rpn){
        ArrayList<String> stack = new ArrayList<>();
        for ( int i = 0; i < rpn.size(); i++){
            String token = rpn.get(i);
            if ( isOperator(token.charAt(0)) ) {
                char op = token.charAt(0);
                String a = stack.remove(stack.size() - 1);
                String b = stack.remove(stack.size() - 1);
                stack.add(getValueOfExpression(b, a, op));
            } else {
                stack.add(token);
            }
        }
        return stack.get(0);
    }

    public static void main(String args[]) {
        System.out.println("90165: " + calculate_rpn(toRpn("27+38+81.9+48*33*53+91*53+82*14+96")));
        System.out.println("616222: " + calculate_rpn(toRpn("22*26*53+66*8+7*76*25*44+78+100")));
        System.out.println(calculate_rpn(toRpn("(22+4)*4")));
    }
}