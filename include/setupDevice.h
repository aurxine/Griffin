#ifndef setupDevice_H
#define setupDevice_H

#include<Arduino.h>
#include<EEPROM.h>
#include<ESP8266WiFi.h>
#define Max_Number_of_Contacts 3
#define Max_Number_of_Sensors 6
#define eeprom_size 256
#define device_id_length 8
#define Max_STA_SSID_Length 20
#define Max_STA_Pass_Length 20
#define Max_AP_SSID_Length 20
#define Max_AP_Pass_Length 20


class  setupDevice
{
    private:
        String device_id; // nodeMCU mac address
        //String device_mode;
        String STA_wifi_ssid; 
        String STA_wifi_password;
        String AP_wifi_ssid;
        String AP_wifi_password;

        // bool locked; // true = locked, false = unlocked

        byte Reset; //first time 1, rest of the time no assignment

        int number_of_contacts; //how many contacts to save
        String contacts[Max_Number_of_Contacts];//string array to save the phone numbers
        int contact_count = 0; 

        String message;

        int number_of_sensors;

        int current_address = 0; //address to save byte data

        int reset_address = current_address++; // 0 
        // int lock_address = current_address++; 

        int STA_ssid_length_address = current_address++; // 1
        int AP_ssid_length_address = current_address++; // 2

        int STA_password_length_address = current_address++; // 3
        int AP_password_length_address = current_address++; // 4

        int custom_message_length_address = current_address++;

        int number_of_contact_address = current_address++; // 5
        int number_of_sensors_address = current_address++;

        int device_id_start_address = current_address++; // 6

        int STA_ssid_start_address = current_address + device_id_length; // 14
        int AP_ssid_start_address = STA_ssid_start_address + Max_STA_SSID_Length; // 34

        int STA_password_start_address = AP_ssid_start_address + Max_AP_SSID_Length; // 54
        int AP_password_start_address = STA_password_start_address + Max_STA_Pass_Length; // 74
        int phone_number_start_address = AP_password_start_address + Max_AP_Pass_Length; // 94
        int custom_message_start_address = phone_number_start_address + 11*Max_Number_of_Contacts; //127


    public:
        setupDevice();
        setupDevice(String id);
        setupDevice(String STA_ssid, String STA_password, String AP_ssid, String AP_password);
        setupDevice(String id, String STA_ssid, String STA_password, String AP_ssid, String AP_password);
        setupDevice(String STA_ssid, String STA_password, String AP_ssid, String AP_password, String number[]);

        bool wifiStatus();
        void showLocalIP();
        void showSoftAPIP();
        void showMacAddress();
        bool connect();
        bool connectToWiFi();
        bool connectToAP();

        bool checkID(String id);
        void getID(String id);
        String ID();
        void showID();

        bool isLocked();
        void setLock(bool lock);

        void get_STA_SSID(String ssid);
        String STA_SSID();
        void show_STA_SSID();

        void get_AP_SSID(String ssid);
        String AP_SSID();
        void show_AP_SSID();

        void get_STA_Password(String password);
        String STA_Password();
        void show_STA_Password();

        void get_AP_Password(String password);
        String AP_Password();
        void show_AP_Password();

        void getNumberOfContacts(int num);
        int NumberOfContacts();
        void showNumberOfContacts();

        void getContact(String phone_number);
        String lastContact();
        String Contact(int n);
        void showContacts();

        void getMessage(String msg);
        String Message();
        void showMessage();

        void getNumberOfSensors(int number);
        int NumberOfSensors();
        void showNumberOfSensors();

        void reset(bool state);
        bool resetState();

        int writeStringInEEPROM(int init_addr, int size, String data);
        int writeByteInEEPROM(int init_addr, byte data);
        byte readByteInEEPRPOM(int address);
        String readStringInEEPRPOM(int init_addr, int size);

        void showAllData();

        //~ setupDevice();
};


#endif