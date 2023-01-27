BUILD_FOLDER = build
PROJECT_NAME = emosy_app
BOARD_NAME = esp8266:esp8266:d1_mini
RYEM_LIB = Ryem
WIFI_MGR_LIB = WifiManager
PROJECT_FOLDER := $(PWD)

$(BUILD_FOLDER)/$(PROJECT_NAME).ino.hex: $(PROJECT_NAME).ino
	arduino-cli compile -b $(BOARD_NAME) \
	--build-path ./$(BUILD_FOLDER) \
	--library ./$(RYEM_LIB) \
	--library  ./$(WIFI_MGR_LIB) \
	--verbose