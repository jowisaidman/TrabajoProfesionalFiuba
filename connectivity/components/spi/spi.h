#include <stdbool.h>

/// @brief Reliably broadcasts a message to all sibling devices in the node. 
/// @param msg Message to broadcast. Maximum message size: 512 bytes.
/// @param len Len of the message to broadcast. 0 < len <= 512.
/// @return true if all siblings have received the message. false in case of error 
bool broadcast_to_siblings(const uint8_t *msg, uint16_t len);