#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include "SocketDatagrama.h"
#include "PaqueteDatagrama.h"
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <thread>
using namespace std;

static const char *s_http_port ;
static struct mg_serve_http_opts s_http_server_opts;
struct timeval t1, t2;

static void handle_http(struct mg_connection *nc, struct http_message *hm, SocketDatagrama &soc) {

		char query[256];
		string response;
		size_t len_response;
		int tim;

		mg_get_http_var(&hm->body, "query", query, sizeof(query));
		printf("IP: %s\n", query);
		gettimeofday(&t1, NULL); //MEDIR TIEMPO DE RESPUESTA

		PaqueteDatagrama paq2(sizeof(tim));

		for (int i = 0; i < 7; i++){
			int n = soc.recibeTimeout(&paq2,0,500000); //REDUCIMOS EL TIEMPO DE ESPERA
			if(n >= 0){
				gettimeofday(&t2, NULL);
				tim = t2.tv_usec - t1.tv_usec;
				response.append(paq2.obtieneDireccion());
				response.append(", Tiempo: ");
				response.append(to_string(tim));
				response.append("\n");
			}
		}
		//sprintf(query, "IP: %s  , Tiempo: %d ", paq2.obtieneDireccion(), tim);
		mg_send_head(nc, 200, response.size(), "Content-Type: text/plain");
		mg_printf(nc, "%s", response.c_str());
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {

	char query[256];
 	struct http_message *hm = (struct http_message *) p;

	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/search") == 0) { 
			
			mg_get_http_var(&hm->body, "query", query,sizeof(query));
			printf("Cadena introducida: %s\n",query);
			PaqueteDatagrama paq1(NULL, 0, query, 7200);
			SocketDatagrama soc(7200);
			soc.envia(&paq1);
		    handle_http(nc, hm, soc);  
		}else{
			mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
		}
	}

}

int main(int argc, char* argv[]) {
	struct mg_mgr mgr;
	struct mg_connection *nc;
	s_http_port = (const char *)argv[1];
	mg_mgr_init(&mgr, NULL);

	printf("Starting web server on port: %s\n", s_http_port);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}
	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "www"; // Serve current directory
	s_http_server_opts.enable_directory_listing = "yes";
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
	return 0;
}
