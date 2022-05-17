#pragma once
#include <mysql.h>
#include <iostream>

using namespace std;

class Class_conexion
{
private:
	MYSQL* conector;
	int status = 0;

public:
	void conexion_open_sql() {
		conector = mysql_init(0);
		conector = mysql_real_connect(conector, "127.0.0.1", "Lab-con", "Lab_con", "bd_facturacion", 3306, NULL, 0);
		//cout << "Conexion establecida!!\n\n\n";
	}
	MYSQL* getConector() {
		return conector;
	}
	void conexion_closed_sql() {
		mysql_close(conector);
		status = 0;
	}
	int Ventas_AIO_max() {
		//Función para devolver el idVenta del ultimo registro y utilizarlo al ingresar el detalle de ventas
		conexion_open_sql();
		MYSQL_ROW fila;
		MYSQL_RES* result;
		int int_max{};

		string max_dat = "SELECT MAX(idVenta) AS idVenta FROM ventas";
		const char* execute_show = max_dat.c_str();
		int q_estado = mysql_query(getConector(), execute_show);
		result = mysql_store_result(getConector());
		while (fila = mysql_fetch_row(result)) {
			int_max = atoi(fila[0]);
		}
		return int_max;
		conexion_closed_sql();
	}
};