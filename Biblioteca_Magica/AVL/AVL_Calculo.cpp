#include  "ArbolAVL.h"
#include  <algorithm>

//calculo para la altura del nodo
/*int ArbolAVL::altura(NodoAVL *nodo) const {
    return nodo ? nodo->altura : 0;
}
*/
//calcular el factor de balance de un nodo
//el calculo es facto= altura(subarbol izquierdo) - altura(subarbol derecho)
/*int ArbolAVL::factorBalance(NodoAVL* nodo) const {
    if (!nodo) return 0;
    return  altura(nodo->izquierdo) -  altura(nodo->derecho);
}*/

//devolver el nodo con el valor nimimo que esta mas a al izquierda
NodoAVL *ArbolAVL::nodoMinimo(NodoAVL *nodo) const {
    NodoAVL* actual = nodo;
    while (actual && actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}

