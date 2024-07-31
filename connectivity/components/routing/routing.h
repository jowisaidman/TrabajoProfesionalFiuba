// TODO: Chequear si vamos a tener un struct o simplemente llamamos estos metodos

/// @brief Function that calls the control plane to communicate 
/// a new node has joined the network
void on_peer_connected();


/// @brief Function that calls the control plane to communicate 
/// a node has left the network
void on_peer_lost();


/// @brief Function that calls the control plane to communicate 
/// that a new message has been received
void on_peer_message();

/// @brief Informs the routing subsystem that a SPI sibling device has sent us a control message.
/// @param msg Message received
/// @param len Length of the message. 0 < len <= 512 
void on_sibling_message(const uint8_t *msg, uint16_t len);