#include "Separator.h"

Separator::Separator()
{

}

String* Separator::SeparatorIndex(String _array,String Separador)
{
    static String array[9]={"","","","","","","","",""};
    int stop;
    int LongArray = _array.length();
    String Array=_array;
    int i = 0;
    int index;
    do
    {
        index = Array.indexOf(Separador);
        array[i] = Array.substring(0,index);
        i++;
        Array=Array.substring(index+1,LongArray);
        stop = Array.length();
    } 
    while (stop>=1 & i<9);
    return array;
}