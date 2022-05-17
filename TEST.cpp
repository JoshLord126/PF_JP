#include <iostream>
#include <mysql.h>
#include "Class_conexion.h"
#include "Ventas_AIO.h"

using namespace std;
Class_conexion obj_conexion;
Ventas_AIO obj_ventas_aio;

int main()
{
    cout << "Testeo, Bienvenido\n\n";
    
    /*
    INGRESO: 
        obj_ventas_aio = Ventas_AIO(NULL, 100, 2, 2, 'B', "2022-05-17", "2022-05-17", NULL, NULL, "*", NULL);
        obj_ventas_aio.Ventas_AIO_ingreso(1);
        int int_idVenta = obj_conexion.Ventas_AIO_max();
        obj_ventas_aio = Ventas_AIO(int_idVenta, NULL, NULL, NULL, NULL, "****-**-**", "****-**-**", NULL, 4, "325", 100.99);
        obj_ventas_aio.Ventas_AIO_ingreso(2);

    */
    //MOSTRAR: 
        obj_ventas_aio.Ventas_AIO_mostrar(1, 2, "11");


    

    
    
}