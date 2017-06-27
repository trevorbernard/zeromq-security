#include <zmq.h>
#include <string.h>
#include <stdio.h>

static void  setup_curve (void * socket, int is_server)
{
    const char *secret_key;
    const char *public_key;
    const char *server_key;

    if (is_server) {
        public_key = ".s&p=^57A5>h-Xgyv%mJX58}C)]fLp9&t{xl[LR]";
        secret_key = "gJ4jDw>ZdY1eASGv2R{<u/ZMB8:$wYG4km:g{lp>";
        server_key = NULL;
    }
    else {
        public_key = "Yne@$w-vo<fVvi]a<NY6T1ed:M$fCG*[IaLV{hID";
        secret_key = "D:)Q[IlAW!ahhC2ac:9*A}h:p?([4%wOTJ%JR%cs";
        server_key = ".s&p=^57A5>h-Xgyv%mJX58}C)]fLp9&t{xl[LR]";
    }

    zmq_setsockopt (socket, ZMQ_CURVE_SECRETKEY, secret_key, strlen(secret_key));
    zmq_setsockopt (socket, ZMQ_CURVE_PUBLICKEY, public_key, strlen(public_key));
    if (is_server)
        zmq_setsockopt (socket, ZMQ_CURVE_SERVER, &is_server, sizeof(is_server));
    else
        zmq_setsockopt (socket, ZMQ_CURVE_SERVERKEY, server_key, strlen(server_key));
}

int main (void)
{
    int rc;
    void *context = zmq_ctx_new ();

    void *server = zmq_socket (context, ZMQ_PUSH);
    setup_curve (server, 1);
    rc = zmq_bind (server, "tcp://*:7210");
    printf("Server bind: %d\n", rc);

    void * client = zmq_socket (context, ZMQ_PULL);
    setup_curve (client, 0);
    rc = zmq_connect (client, "tcp://127.0.0.1:7210");
    printf("Client connect: %d\n", rc);
    rc = zmq_send (server, "Hello", 6, 0);
    printf("Send %d\n", rc);
    char buff[6];
    int len = zmq_recv(client, buff, 6, 0);
    if (len > 0) {
        puts(buff);
    }

    zmq_close (server);
    zmq_close (client);
    zmq_ctx_destroy (context);

    return 0;
}
