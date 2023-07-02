#!/bin/bash

fetch_version_from_gist() {
    curl -H "Authorization: token $GISTS_TOKEN" "https://api.github.com/gists/ee91a5f2c84cd2d4841b37e9db19a8f0" | jq -r '.files["VERSION"].content'
}

update_version_in_gist() {
    local current_version=$1
    curl -X PATCH -H "Authorization: token $GISTS_TOKEN" -d "{\"files\": {\"VERSION\": {\"content\": \"$current_version\"}}}" "https://api.github.com/gists/ee91a5f2c84cd2d4841b37e9db19a8f0" > /dev/null
}

cd /home/ci/project
pio lib install

build_output=$(pio run -e megaatmega2560)

ram_usage=$(echo "$build_output" | grep "RAM: " | awk '{print $4}')
flash_usage=$(echo "$build_output" | grep "Flash: " | awk '{print $4}')

echo "RAM Usage: $ram_usage"
echo "Flash Usage: $flash_usage"

curl -X PATCH -H "Authorization: token $GISTS_TOKEN" -d "{\"files\": {\"badges.json\": {\"content\": \"{ \\\"schemaVersion\\\": 1, \\\"label\\\": \\\"RAM usage\\\", \\\"message\\\": \\\"$ram_usage\\\", \\\"color\\\": \\\"green\\\" }\"}}}" "https://api.github.com/gists/df02b7d3aa818b19a80c7f3787f948d7" > /dev/null
curl -X PATCH -H "Authorization: token $GISTS_TOKEN" -d "{\"files\": {\"badges2.json\": {\"content\": \"{ \\\"schemaVersion\\\": 1, \\\"label\\\": \\\"Flash usage\\\", \\\"message\\\": \\\"$flash_usage\\\", \\\"color\\\": \\\"green\\\" }\"}}}" "https://api.github.com/gists/995ca8a8c65a37140d11be684146bee0" > /dev/null

LAST_VERSION=$(fetch_version_from_gist)
CURRENT_VERSION=$(cat VERSION)

if [ "$CURRENT_VERSION" != "$LAST_VERSION" ]; then
    echo "Version has changed from $LAST_VERSION to $CURRENT_VERSION"
    # Update the version in gist
    update_version_in_gist $CURRENT_VERSION
    # Use exit code 1 to indicate that VERSION has changed
    exit 1
else
    # Use exit code 0 to indicate that VERSION has not changed
    exit 0
fi
