#!/bin/bash
# TARGET_DIR is the root of the Pi's filesystem during the build process

echo "Applying Zero-Touch Industrial Provisioning..."

# 1. FIX PERMISSIONS FOR MOSQUITTO
# In Buildroot, the 'mosquitto' user usually has UID 188. 
# We change ownership of the certs so the broker can read them after dropping privileges.
mkdir -p ${TARGET_DIR}/etc/mosquitto/certs
chown -R 188:188 ${TARGET_DIR}/etc/mosquitto/certs
chmod 600 ${TARGET_DIR}/etc/mosquitto/certs/gateway.key

# 2. SETUP LOGGING
# Ensure the log file exists and is writable by the mosquitto user
mkdir -p ${TARGET_DIR}/var/log
touch ${TARGET_DIR}/var/log/mosquitto.log
chown 188:188 ${TARGET_DIR}/var/log/mosquitto.log

# 3. ENABLE SYSTEMD SERVICES
# This creates the 'Auto-start' links for both your service and the networking
mkdir -p ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants
ln -sf /etc/systemd/system/gateway_service.service \
       ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants/gateway_service.service
ln -sf /lib/systemd/system/systemd-networkd.service \
       ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants/systemd-networkd.service
ln -sf /lib/systemd/system/systemd-resolved.service \
       ${TARGET_DIR}/etc/systemd/system/multi-user.target.wants/systemd-resolved.service

# 4. REMOVE SERIAL CONSOLE CONFLICT
# This stops the 'ttyS0' spam from blocking the boot process
rm -f ${TARGET_DIR}/etc/systemd/system/getty.target.wants/getty@ttyS0.service

# 5. HARDEN SSH ACCESS
if [ -f ${TARGET_DIR}/etc/ssh/sshd_config ]; then
    sed -i 's/#PermitRootLogin.*/PermitRootLogin yes/' ${TARGET_DIR}/etc/ssh/sshd_config
fi

# 6. SET SYSTEM IDENTITY
echo "industrial-gateway-01" > ${TARGET_DIR}/etc/hostname
