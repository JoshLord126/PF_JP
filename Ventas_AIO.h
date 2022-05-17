#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
#include "Class_conexion.h"

using namespace std;

class Ventas_AIO
{
protected:
	//Variables a utilizar en **Ventas**
	int int_nofactura, int_idcliente, int_idempleado;
	char chr_serie[1];
	string str_fechafactura, str_fechaingreso;

	//Variables a utilizar en **Ventas_detalle**
	int int_idventa_detalle, int_idProducto;
	string str_cantidad;
	float flt_precio_unitario;

	//Variables en común
	int int_idVenta;

public:
	Ventas_AIO() {
	}

	Ventas_AIO(int int_idven, int int_nofac, int int_idcli, int int_idemp, char chr_ser, string str_fechafac, string str_fechain, int int_idven_det, int int_idpro, string str_can, float flt_pre) {
		int_idVenta = int_idven;
		int_nofactura = int_nofac;
		int_idcliente = int_idcli;
		int_idempleado = int_idemp;
		chr_serie[0] = chr_ser;
		str_fechafactura = str_fechafac;
		str_fechaingreso = str_fechain;
		int_idventa_detalle = int_idven_det;
		int_idProducto = int_idpro;
		str_cantidad = str_can;
		flt_precio_unitario = flt_pre;
	}

	Class_conexion obj_conexion;

	void Ventas_AIO_ingreso(int int_fase) {
		obj_conexion.conexion_open_sql();
		//Fase 1 = Ingreso de Ventas; Fase 2 = Ingreso del detalle de ventas. 
		string str_idVenta = to_string(int_idVenta);
		if (int_fase == 1) {
			string str_nofactura = to_string(int_nofactura);
			string str_idcliente = to_string(int_idcliente);
			string str_idempleado = to_string(int_idempleado);
			string str_serie(chr_serie);
			//(FechaFactura, FechaIngreso, Cantidad) = String, por lo que ya no se hace la conversión.
			
			string insert_dat = "insert into ventas(nofactura, serie, fechafactura, idCliente, idEmpleado, fechaingreso) values(" + str_nofactura + ", '" + str_serie + "', '" + str_fechafactura + "', " + str_idcliente + ", " + str_idempleado + ", '" + str_fechaingreso + "')";
			const char* execute_insert = insert_dat.c_str();
			int q_estado = mysql_query(obj_conexion.getConector(), execute_insert);
			if (!q_estado) {
				cout << "\n\nIngreso OK\n\n";
			}
			else {
				cout << "\n\nIngreso F\n\n";
			}
		}
		else if (int_fase == 2) {
			string str_precio_unitario = to_string(flt_precio_unitario);
			string str_idProducto = to_string(int_idProducto);
			string str_idventa_detalle = to_string(int_idventa_detalle);

			string insert_dat = "insert into ventas_detalle(idVenta, idProducto, cantidad, precio_unitario) values(" + str_idVenta + ", " + str_idProducto + ", '" + str_cantidad + "', " + str_precio_unitario + ")";
			const char* execute_insert = insert_dat.c_str();
			int q_estado = mysql_query(obj_conexion.getConector(), execute_insert);
			if (!q_estado) {
				cout << "\n\nIngreso OK\n\n";
			}
			else {
				cout << "\n\nIngreso F\n\n";
			}
		}
		else {
			cout << "\n\nError de fase: Fase inexistente.\n\n";
		}
		
		obj_conexion.conexion_closed_sql();
	}

	void Ventas_AIO_mostrar(int int_opcion, int int_variable, string str_id) {
		/*
		Combinaciones:
			(int_opcion = 1 && int_variable = 1) = Mostrar solo tabla Venta. 
			(int_opcion = 2 && int_variable = 1) = Mostrar solo tabla Venta_Detalle.
			(int_opcion = 1 && int_variable = 2 && str_id = "x") = Filtra por idVenta sobre ambas tablas.
		*/
		if (int_opcion == 1 || int_opcion == 2) {
			if (int_variable == 1 || int_variable == 2) {
				obj_conexion.conexion_open_sql();
				MYSQL_ROW fila;
				MYSQL_RES* result;
				string show_dat;

				do {
					show_dat = "select * from ";
					if (int_opcion == 1) {
						if (int_variable == 1) {
							show_dat = show_dat + "ventas";
						}
						else if (int_variable == 2) {
							show_dat = "select idVenta, nofactura, serie, fechafactura, concat(clientes.nombres, ' ',clientes.apellidos) as Cliente, concat(empleados.nombres, ' ', empleados.apellidos) as Empleado, ventas.fechaingreso from ventas inner join clientes on clientes.idCliente=ventas.idCliente inner join empleados on empleados.idEmpelado=ventas.idEmpleado where idVenta=" + str_id;
						}
					}
					else if (int_opcion == 2) {
						if (int_variable == 1) {
							show_dat = show_dat + "ventas_detalle";
						}
						else if (int_variable == 2) {
							show_dat = "select ventas_detalle.idVenta_detalle, ventas_detalle.idVenta, productos.producto, ventas_detalle.cantidad, ventas_detalle.precio_unitario from ventas_detalle INNER JOIN productos ON productos.idProducto=ventas_detalle.idProducto where idVenta=" + str_id;
						}
					}
					else if (int_opcion == 3) {
						show_dat = "select sum(precio_unitario) as Total from ventas_detalle where idVenta=" + str_id;
					}

					const char* execute_show = show_dat.c_str();
					int q_estado = mysql_query(obj_conexion.getConector(), execute_show);
					if (!q_estado) {
						int i = 1;
						result = mysql_store_result(obj_conexion.getConector());
						if (int_opcion == 1) {
							cout << "No. | ID, No.Factura, Serie, FechaFactura, ID-Cliente, ID-Empleado, Fecha-Ingreso |" << endl;
						}
						else if (int_opcion == 2) {
							cout << "No. | ID, ID-Venta, ID-Producto, Cantidad, PrecioUnitario |" << endl;
						}
						while (fila = mysql_fetch_row(result)) {
							if (int_opcion == 1) {
								cout << i << " | " << fila[0] << ", " << fila[1] << ", " << fila[2] << ", " << fila[3] << ", " << fila[4] << ", " << fila[5] << ", " << fila[6] << " | " << endl;
							}
							else if (int_opcion == 2) {
								cout << i << " | " << fila[0] << ", " << fila[1] << ", " << fila[2] << ", " << fila[3] << ", " << fila[4] << " | " << endl;
							}
							else if (int_opcion == 3) {
								cout << "Total: " << fila[0] << endl;
							}
							i++;
						}
					}

					if (int_opcion <= 3 && int_variable == 2) {
						int_opcion++;
					}
					else {
						int_opcion = 4;
					}
				} while (int_opcion < 4);
				obj_conexion.conexion_closed_sql();
			}
			else {
				cout << "Por favor ingrese una combinación valida en la variable.";
			}
		}
		else {
			cout << "Por favor ingrese una combinación valida en la opcion.";
		}
	}
};