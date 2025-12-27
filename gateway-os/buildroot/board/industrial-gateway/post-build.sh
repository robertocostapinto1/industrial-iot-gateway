#!/bin/bash
# TARGET_DIR is a variable Buildroot provides automatically

echo "FIXING PERMISSIONS: Securing the Gateway Private Key..."

# Secure the private key so ONLY the mosquitto user can see it
# In Buildroot, the mosquitto user usually has UID 188
if [ -f ${TARGET_DIR}/etc/mosquitto/certs/gateway.key ]; then
    chmod 600 ${TARGET_DIR}/etc/mosquitto/certs/gateway.key
fi

# Set a custom hostname so your Pi identifies itself on the network
echo "industrial-gateway-01" > ${TARGET_DIR}/etc/hostname
