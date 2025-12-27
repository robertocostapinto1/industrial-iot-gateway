#!/bin/bash
# TARGET_DIR is a variable Buildroot provides automatically

echo "Applying Industrial Hardening & Service Configuration..."

# Secure the private key so ONLY the mosquitto user can see it
# In Buildroot, the mosquitto user usually has UID 188
if [ -f ${TARGET_DIR}/etc/mosquitto/certs/gateway.key ]; then
    chmod 600 ${TARGET_DIR}/etc/mosquitto/certs/gateway.key
fi

# Enable the Gateway Service in Systemd
# this creates the link tht starts the app on boot
mkdir -p ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants
ln -sf /etc/systemd/system/gateway_service.service \
       ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants/gateway_service.service

# Set a custom hostname so your Pi identifies itself on the network
echo "industrial-gateway-01" > ${TARGET_DIR}/etc/hostname
