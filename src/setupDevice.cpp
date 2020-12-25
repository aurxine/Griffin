#include"setupDevice.h"
#include<Arduino.h>
#include<EEPROM.h>
#include<ESP8266WiFi.h>


setupDevice::setupDevice()
{
    if(this->readByteInEEPRPOM(reset_address) == 0)
    {
        device_id = this->readStringInEEPRPOM(device_id_start_address, device_id_length);

        STA_wifi_ssid = this->readStringInEEPRPOM(STA_ssid_start_address, 
        this->readByteInEEPRPOM(STA_ssid_length_address));

        STA_wifi_password = this->readStringInEEPRPOM(STA_password_start_address, 
        this->readByteInEEPRPOM(STA_password_length_address));

        AP_wifi_ssid = this->readStringInEEPRPOM(AP_ssid_start_address, 
        this->readByteInEEPRPOM(AP_ssid_length_address));

        AP_wifi_password = this->readStringInEEPRPOM(AP_password_start_address, 
        this->readByteInEEPRPOM(AP_password_length_address));

        number_of_contacts = this->readByteInEEPRPOM(number_of_contact_address);
        for(int i = 0; i<number_of_contacts; i++)
        {
            contacts[i] = this->readStringInEEPRPOM(phone_number_start_address+11*i, 11);
        }

        // message = "Breach Alert!";
        // this->getMessage(message);
    }
    else
    {
        for(int i = 1; i<eeprom_size; i++)
        {
            this->writeByteInEEPROM(i, 0);
        }
    }
}

setupDevice::setupDevice(String id)
{
    device_id = id;
    this->writeStringInEEPROM(device_id_start_address, device_id_length, device_id);

    if(this->readByteInEEPRPOM(reset_address) == 0)
    {
        STA_wifi_ssid = this->readStringInEEPRPOM(STA_ssid_start_address, 
        this->readByteInEEPRPOM(STA_ssid_length_address));

        STA_wifi_password = this->readStringInEEPRPOM(STA_password_start_address, 
        this->readByteInEEPRPOM(STA_password_length_address));

        AP_wifi_ssid = this->readStringInEEPRPOM(AP_ssid_start_address, 
        this->readByteInEEPRPOM(AP_ssid_length_address));

        AP_wifi_password = this->readStringInEEPRPOM(AP_password_start_address, 
        this->readByteInEEPRPOM(AP_password_length_address));

        number_of_contacts = this->readByteInEEPRPOM(number_of_contact_address);
        for(int i = 0; i<number_of_contacts; i++)
        {
            contacts[i] = this->readStringInEEPRPOM(phone_number_start_address+11*i, 11);
        }
    }
    else
    {
        for(int i = 1; i<eeprom_size; i++)
        {
            this->writeByteInEEPROM(i, 0);
        }
    }
    
}

setupDevice::setupDevice(String STA_ssid, String STA_password, String AP_ssid, String AP_password)
{
    device_id = this->readStringInEEPRPOM(device_id_start_address, device_id_length);

    STA_wifi_ssid = STA_ssid;
    STA_wifi_password = STA_password;
    AP_wifi_ssid = AP_ssid;
    AP_wifi_password = AP_password;

    int STA_ssid_length = STA_ssid.length();
    int STA_pass_length = STA_password.length();
    int AP_ssid_length = AP_ssid.length();
    int AP_pass_length = AP_password.length();

    this->writeByteInEEPROM(reset_address, 0);
    this->writeByteInEEPROM(STA_ssid_length_address, STA_ssid_length);
    this->writeByteInEEPROM(STA_password_length_address, STA_pass_length);
    this->writeByteInEEPROM(AP_ssid_length_address, AP_ssid_length);
    this->writeByteInEEPROM(AP_password_length_address, AP_pass_length);

    current_address = writeStringInEEPROM(STA_ssid_start_address, STA_ssid_length, STA_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(STA_password_start_address, STA_pass_length, STA_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }

    current_address = writeStringInEEPROM(AP_ssid_start_address, AP_ssid_length, AP_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(AP_password_start_address, AP_pass_length, AP_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }
    
}

setupDevice::setupDevice(String id, String STA_ssid, String STA_password, String AP_ssid, String AP_password)
{
    device_id = id;
    this->writeStringInEEPROM(device_id_start_address, device_id_length, device_id);

    STA_wifi_ssid = STA_ssid;
    STA_wifi_password = STA_password;
    AP_wifi_ssid = AP_ssid;
    AP_wifi_password = AP_password;

    int STA_ssid_length = STA_ssid.length();
    int STA_pass_length = STA_password.length();
    int AP_ssid_length = AP_ssid.length();
    int AP_pass_length = AP_password.length();

    this->writeByteInEEPROM(reset_address, 0);
    this->writeByteInEEPROM(STA_ssid_length_address, STA_ssid_length);
    this->writeByteInEEPROM(STA_password_length_address, STA_pass_length);
    this->writeByteInEEPROM(AP_ssid_length_address, AP_ssid_length);
    this->writeByteInEEPROM(AP_password_length_address, AP_pass_length);

    current_address = writeStringInEEPROM(STA_ssid_start_address, STA_ssid_length, STA_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(STA_password_start_address, STA_pass_length, STA_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }

    current_address = writeStringInEEPROM(AP_ssid_start_address, AP_ssid_length, AP_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(AP_password_start_address, AP_pass_length, AP_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }
    
}

setupDevice::setupDevice(String STA_ssid, String STA_password, String AP_ssid, String AP_password, String number[])
{
    number_of_contacts = sizeof(number)/sizeof(number[0]);
    for(int i = 0; i<number_of_contacts; i++)
    {
        contacts[i] = number[i];
    }
    device_id = "Griffin0";
    STA_wifi_ssid = STA_ssid;
    STA_wifi_password = STA_password;
    AP_wifi_ssid = AP_ssid;
    AP_wifi_password = AP_password;

    int STA_ssid_length = STA_ssid.length();
    int STA_pass_length = STA_password.length();
    int AP_ssid_length = AP_ssid.length();
    int AP_pass_length = AP_password.length();

    this->writeByteInEEPROM(reset_address, 0);
    this->writeByteInEEPROM(STA_ssid_length_address, STA_ssid_length);
    this->writeByteInEEPROM(STA_password_length_address, STA_pass_length);
    this->writeByteInEEPROM(AP_ssid_length_address, AP_ssid_length);
    this->writeByteInEEPROM(AP_password_length_address, AP_pass_length);

    current_address = writeStringInEEPROM(STA_ssid_start_address, STA_ssid_length, STA_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(STA_password_start_address, STA_pass_length, STA_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }

    current_address = writeStringInEEPROM(AP_ssid_start_address, AP_ssid_length, AP_ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }

    current_address = writeStringInEEPROM(AP_password_start_address, AP_pass_length, AP_password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }

    for(int i = 0; i<number_of_contacts; i++)
    {
        current_address = writeStringInEEPROM(phone_number_start_address + 11*i, 11, contacts[i]);
        if(current_address == -1)
        {
            Serial.println("Error saving contacts");
        }
    }
    
}


bool setupDevice::wifiStatus()
{
    if(WiFi.status() == WL_CONNECTED)
        return true;
    return false;
}

void setupDevice::showLocalIP()
{
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
}

void setupDevice::showSoftAPIP()
{
    Serial.print("Soft AP IP: "); Serial.println(WiFi.softAPIP());
}


void setupDevice::showMacAddress()
{
    Serial.print("Mac Address: "); Serial.println(WiFi.macAddress());
}

bool setupDevice::connect()
{   
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(this->AP_SSID(), this->AP_Password());
    WiFi.begin(this->STA_SSID(), this->STA_Password());

    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        count++;
        if(count >= 20)
            break;
    }
    return this->wifiStatus();
}

bool setupDevice::connectToWiFi() // called after AP
{
    String ssid, password;
    ssid = this->STA_SSID();
    password = this->STA_Password();
    // WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        count++;
        if(count >= 20)
            break;
    }

    return this->wifiStatus();
}
bool setupDevice::connectToAP()
{
    String ssid, password;
    ssid = this->AP_SSID();
    password = this->AP_Password();

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, password);
    return true;
}

int setupDevice::writeStringInEEPROM(int init_addr, int size, String data)
{
    EEPROM.begin(eeprom_size);
    int i, j=0;
    //Serial.println("size"+String(size));
    for(i = init_addr; i<(size + init_addr); i++,j++)
    {
        EEPROM.write(i, data[j]);
        //Serial.println(data[j]);
        if(EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
        }
        else 
        {
            Serial.println("ERROR! EEPROM commit failed");
            return -1;
        }
    }
    return i; //new address
}

int setupDevice::writeByteInEEPROM(int init_addr, byte data)
{
    EEPROM.begin(eeprom_size);
    EEPROM.write(init_addr, data);
        if(EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
        }
        else 
        {
            Serial.println("ERROR! EEPROM commit failed");
            return -1;
        }
    return ++init_addr; //new address
}

byte setupDevice::readByteInEEPRPOM(int address)
{
    EEPROM.begin(eeprom_size);
    byte data;
    data = EEPROM.read(address);
    //Serial.println("Data: "+String(data));
    return data;
}

String setupDevice::readStringInEEPRPOM(int init_addr, int size)
{
    EEPROM.begin(eeprom_size);
    String str;
    byte data;
    for(int i=init_addr; i<(size + init_addr); i++)
    {
        data = EEPROM.read(i);
        //Serial.println((char)data);
        str+=(char)data;
    }
    //Serial.println("String: " + str);
    return str;
}

bool setupDevice::checkID(String id)
{
    device_id = this->readStringInEEPRPOM(device_id_start_address, device_id_length);
    return id.compareTo(device_id)==0?true:false;
}

void setupDevice::getID(String id)
{
    device_id = id;
    this->writeStringInEEPROM(device_id_start_address, device_id_length, device_id);
}

String setupDevice::ID()
{
    device_id = this->readStringInEEPRPOM(device_id_start_address, device_id_length);
    return device_id;
}

void setupDevice::showID()
{
    // Serial.println(device_id_start_address);
    // Serial.println(device_id_length);
    // Serial.println(device_id);
    device_id = this->readStringInEEPRPOM(device_id_start_address, device_id_length);
    Serial.print("Device ID: ");
    Serial.println(device_id);
}


void setupDevice::get_STA_SSID(String ssid)
{
    STA_wifi_ssid = ssid;
    this->writeByteInEEPROM(STA_ssid_length_address, ssid.length());
    this->writeStringInEEPROM(STA_ssid_start_address, ssid.length(), ssid);
}

String setupDevice::STA_SSID()
{
    return STA_wifi_ssid;
}

void setupDevice::show_STA_SSID()
{
    int ssid_length = this->readByteInEEPRPOM(STA_ssid_length_address);
    String ssid = this->readStringInEEPRPOM(STA_ssid_start_address, ssid_length);
    Serial.print("Station SSID: ");
    Serial.println(ssid);
}

void setupDevice::get_AP_SSID(String ssid)
{
    AP_wifi_ssid = ssid;
    this->writeByteInEEPROM(AP_ssid_length_address, ssid.length());
    this->writeStringInEEPROM(AP_ssid_start_address, ssid.length(), ssid);
}

String setupDevice::AP_SSID()
{
    return AP_wifi_ssid;
}

void setupDevice::show_AP_SSID()
{
    int ssid_length = this->readByteInEEPRPOM(AP_ssid_length_address);
    String ssid = this->readStringInEEPRPOM(AP_ssid_start_address, ssid_length);
    Serial.print("Access Point SSID: ");
    Serial.println(ssid);
}

void setupDevice::get_STA_Password(String password)
{
    STA_wifi_password = password;
    this->writeByteInEEPROM(STA_password_length_address, password.length());
    this->writeStringInEEPROM(STA_password_start_address, password.length(), password);
}

String setupDevice::STA_Password()
{
    return STA_wifi_password;
}

void setupDevice::show_STA_Password()
{
    int pass_length = this->readByteInEEPRPOM(STA_password_length_address);
    String password = this->readStringInEEPRPOM(STA_password_start_address, pass_length);
    Serial.print("Station Password: ");
    Serial.println(password);
}

void setupDevice::get_AP_Password(String password)
{
    STA_wifi_password = password;
    this->writeByteInEEPROM(AP_password_length_address, password.length());
    this->writeStringInEEPROM(AP_password_start_address, password.length(), password);
}

String setupDevice::AP_Password()
{
    return AP_wifi_password;
}

void setupDevice::show_AP_Password()
{
    int pass_length = this->readByteInEEPRPOM(AP_password_length_address);
    String password = this->readStringInEEPRPOM(AP_password_start_address, pass_length);
    Serial.print("Access Point Password: ");
    Serial.println(password);
}

void setupDevice::getNumberOfContacts(int num)
{
    number_of_contacts = num;
    this->writeByteInEEPROM(number_of_contact_address, num);
}

int setupDevice::NumberOfContacts()
{
    return number_of_contacts;
}

void setupDevice::showNumberOfContacts()
{
    Serial.print("Total number of contacts: ");
    Serial.println(number_of_contacts);
}

void setupDevice::getContact(String phone_number)
{
    if(contact_count<Max_Number_of_Contacts)
    {
        contacts[contact_count] = phone_number;
        this->writeStringInEEPROM(phone_number_start_address + 11*contact_count, 11, phone_number);
        contact_count++;
    }
    else
    {
        Serial.println("Reached maximum phone number");
    }
}

String setupDevice::lastContact()
{
    return contacts[contact_count];
}

String setupDevice::Contact(int n)
{
    if(n<Max_Number_of_Contacts)
    return contacts[n];
    return "";
}

void setupDevice::showContacts()
{
    int n = readByteInEEPRPOM(number_of_contact_address);
    String contacts;
    for(int i = 0; i<n; i++)
    {
        contacts = this->readStringInEEPRPOM(phone_number_start_address+11*i, 11);
        Serial.print("Contact number ");
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(contacts);
    }
}

void setupDevice::reset(bool state)
{
    Reset = state;
    this->writeByteInEEPROM(reset_address, state);
}

bool setupDevice::resetState()
{
    return this->readByteInEEPRPOM(reset_address);
}


void setupDevice::getMessage(String msg)
{
    this->message = msg;
    int msg_length = msg.length();
    this->writeByteInEEPROM(custom_message_length_address, msg_length);
    this->writeStringInEEPROM(custom_message_start_address, msg_length, msg);
    Serial.println(message);
}


String setupDevice::Message()
{
    int msg_length = this->readByteInEEPRPOM(custom_message_length_address);
    String msg = this->readStringInEEPRPOM(custom_message_start_address, msg_length);
    this->message = msg;
    return msg;
}


void setupDevice::showMessage()
{
    String msg = this->Message();
    Serial.print("Message to send: ");
    Serial.println(msg);
}

void setupDevice::getNumberOfSensors(int number)
{
    number_of_sensors = number;
    this->writeByteInEEPROM(number_of_sensors_address, number);
}


int setupDevice::NumberOfSensors()
{
    number_of_sensors = this->readByteInEEPRPOM(number_of_sensors_address);
    return number_of_sensors;
}


void setupDevice::showNumberOfSensors()
{
    number_of_sensors = this->NumberOfSensors();
    Serial.print("Number of sensors: ");
    Serial.println(number_of_sensors);
}

void setupDevice::showAllData()
{
    this->showID();
    delay(500);
    this->show_AP_SSID();
    delay(500);
    this->show_AP_Password();
    delay(500);
    this->show_STA_SSID();
    delay(500);
    this->show_STA_Password();
    delay(500);
    this->showNumberOfSensors();
    delay(500);
    this->showNumberOfContacts();
    delay(500);
    this->showContacts();
    delay(500);
    this->showMessage();
}
