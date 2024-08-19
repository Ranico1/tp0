#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

    valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

    log_info(logger, "La ip es: %s , y el puerto es: %s", ip, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	bool is_active_console = true; 
	nuevo_logger = log_create("/home/utnso/tp0/tp0.log", "tp0" , is_active_console, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) 
	exit(1);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
    nuevo_config = config_create("/home/utnso/tp0/client/cliente.config") ;

	if(nuevo_config == NULL)
	{
		printf("ERROR CONFIG");
		exit(2);
	}	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
while (1) {
        leido = readline("> ");
        if (leido) {
            add_history(leido);
        }
        if (!strncmp(leido, "exit", 4)) {
            free(leido);
            break;
        }
        printf("%s\n", leido);

		log_info(logger, "Leido: %s", leido);


	// ¡No te olvides de liberar las lineas antes de regresar!

free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
paquete = crear_paquete();

	while (1) {
			leido = readline("> ");
			if (leido) {
				add_history(leido);
			}
			if (!strncmp(leido, "exit", 4)) {
				free(leido);
				break;
			}
			printf("%s\n", leido);

			agregar_a_paquete(paquete, leido, sizeof(leido));

			free(leido);
		}	

enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  	
	log_destroy(logger);

	config_destroy(config);

	close(conexion);
}
