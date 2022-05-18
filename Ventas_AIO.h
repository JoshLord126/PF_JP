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
		int q_estado;
		if (int_fase == 1) {
			string str_nofactura = to_string(int_nofactura);
			string str_idcliente = to_string(int_idcliente);
			string str_idempleado = to_string(int_idempleado);
			string str_serie(chr_serie);
			//(FechaFactura, FechaIngreso, Cantidad) = String, por lo que ya no se hace la conversión.
			
			string insert_dat = "insert into ventas(nofactura, serie, fechafactura, idCliente, idEmpleado, fechaingreso) values(" + str_nofactura + ", '" + str_serie + "', '" + str_fechafactura + "', " + str_idcliente + ", " + str_idempleado + ", '" + str_fechaingreso + "')";
			const char* execute_insert = insert_dat.c_str();
			q_estado = mysql_query(obj_conexion.getConector(), execute_insert);
		}
		else if (int_fase == 2) {
			string str_precio_unitario = to_string(flt_precio_unitario);
			string str_idProducto = to_string(int_idProducto);
			string str_idventa_detalle = to_string(int_idventa_detalle);

			string insert_dat = "insert into ventas_detalle(idVenta, idProducto, cantidad, precio_unitario) values(" + str_idVenta + ", " + str_idProducto + ", '" + str_cantidad + "', " + str_precio_unitario + ")";
			const char* execute_insert = insert_dat.c_str();
			q_estado = mysql_query(obj_conexion.getConector(), execute_insert);
		}
		else {
			cout << "\n\nError: Fase inexistente.\n\n";
		}
		
		if (!q_estado) {
			cout << "\n\nValores registrados\n\n";
		}
		else {
			cout << "\n\nError: "<< mysql_error(obj_conexion.getConector());
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
					else {
						cout << "\n\nError: Por favor, ingrese un ID correcto.\n\n";
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
				cout << "Error: Por favor ingrese una combinación valida en la variable.";
			}
		}
		else {
			cout << "Error: Por favor ingrese una combinación valida en la opcion.";
		}
	}

	void Ventas_AIO_eliminar(string str_id, int int_tabla) {
		/*
		(str_id) = ID a eliminar.
		(int_tabla = 1) = Tabla Ventas.
		(int_tabla = 2) = Tabla Ventas_detalle.
		*/
		string delete_dat = "delete from ";
		obj_conexion.conexion_open_sql();
		
		if (int_tabla == 1) {
			delete_dat = delete_dat + "ventas where idVenta=" + str_id;
		}
		else if (int_tabla == 2) {
			delete_dat = delete_dat + "ventas_detalle where idVenta_detalle=" + str_id;
		}
		else {
			cout << "\n\nError: Ingrese un valor de tabla correcto.\n\n";
		}
		
		const char* execute_delete = delete_dat.c_str();
		int q_estado = mysql_query(obj_conexion.getConector(), execute_delete);
		
		if (!q_estado) {
			cout << "\n\nEliminado\n\n";
		}
		else {
			cout << "\n\nError: ";
			const char* chr_errorbase = "Cannot delete or update a parent row: a foreign key constraint fails";
			const char* chr_error = mysql_error(obj_conexion.getConector());
			int int_error = strcmp(chr_errorbase, chr_error);

			if (int_error == 0) {
				cout << "Por favor elimine todos los registros asociados a la venta. ";
			}
			else {
				cout << "Por favor verique que el ID exista o comuniquese con su administrador";
			};
		}
		obj_conexion.conexion_closed_sql();
	}

	void Ventas_AIO_update(string str_id, string str_columna, string str_nuevo_valor, int int_tabla) {
		/*
		(str_id) = ID a actualizar. 
		(str_columna) = Columna a modificar sus registros.
		(str_nuevo_valor) = Nuevo valor.
		(int_tabla = 1) = Tabla Ventas.
		(int_tabla = 2) = Tabla Ventas_detalle.
		*/
		string update_dat;
		obj_conexion.conexion_open_sql();
		
		if (int_tabla == 1) {
			update_dat = "update ventas set " + str_columna + "='" + str_nuevo_valor + "' where idVenta=" + str_id;
		}
		else if (int_tabla == 2) {
			update_dat = "update ventas_detalle set " + str_columna + "='" + str_nuevo_valor + "' where idVenta_detalle=" + str_id;
		}
		else {
			cout << "Error: Ingrese un valor de tabla correcto";
		}
		
		const char* execute_update = update_dat.c_str();
		int q_estado = mysql_query(obj_conexion.getConector(), execute_update);
		if (!q_estado) {
			cout << "\n\nActualizado\n\n";
		}
		else {
			cout << "\n\nError: " << mysql_error(obj_conexion.getConector());
		}
		obj_conexion.conexion_closed_sql();
	}
};