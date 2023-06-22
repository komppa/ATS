#!/bin/bash

cd /home/ci/project
pio lib install

build_output=$(pio run -e megaatmega2560)

ram_usage=$(echo "$build_output" | grep "RAM: " | awk '{print $4}')
flash_usage=$(echo "$build_output" | grep "Flash: " | awk '{print $4}')

echo "RAM Usage: $ram_usage"
echo "Flash Usage: $flash_usage"

curl -X PATCH -H "Authorization: token $GISTS_TOKEN" -d "{\"files\": {\"badges.json\": {\"content\": \"{ \\\"schemaVersion\\\": 1, \\\"label\\\": \\\"RAM usage\\\", \\\"message\\\": \\\"$ram_usage\\\", \\\"color\\\": \\\"green\\\" }\"}}}" "https://api.github.com/gists/df02b7d3aa818b19a80c7f3787f948d7" > /dev/null
curl -X PATCH -H "Authorization: token $GISTS_TOKEN" -d "{\"files\": {\"badges2.json\": {\"content\": \"{ \\\"schemaVersion\\\": 1, \\\"label\\\": \\\"Flash usage\\\", \\\"message\\\": \\\"$flash_usage\\\", \\\"color\\\": \\\"green\\\" }\"}}}" "https://api.github.com/gists/995ca8a8c65a37140d11be684146bee0" > /dev/null