#include "./scanner.h"

typedef struct _Expr {
    struct Expr * left;
    struct Expr * right;
    Token operator;
} Expr;

// abstract class Expr { 
//   static class Binary extends Expr {
//     Binary(Expr left, Token operator, Expr right) {
//       this.left = left;
//       this.operator = operator;
//       this.right = right;
//     }
// 
//     final Expr left;
//     final Token operator;
//     final Expr right;
//   }
// 
//   // Other expressions...
// }
