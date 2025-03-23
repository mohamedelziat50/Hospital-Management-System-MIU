#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

// Global Variables / Constants
const string FILENAME = "hospital.dat";
const int maxNameLength = 30 + 1; // +1 for NULL Character
const int roomPrice = 100;

// Patient Structure
struct Patient 
{
    int id;
    char name[maxNameLength];
    int roomNumber = -1;
    int durationOfStayInDays = -1;
    int bill;
};

// Prototypes
vector<Patient> readFileIntoVector();
void writeVectorIntoFile(vector<Patient> &PATIENTS);
void displayPatient(Patient &patient);
void displayPatients(vector<Patient> &PATIENTS);
void addPatient(vector<Patient> &PATIENTS);
int calculatePatientId(vector<Patient> &PATIENTS);
void searchForPatient(vector<Patient> &PATIENTS);
void dischargePatient(vector<Patient> &PATIENTS);
void assignRoom(Patient &patient, vector<Patient> &PATIENTS);
bool checkRoomNumber(int roomNumber, const vector<Patient> &PATIENTS);
void calculateBill(Patient &patient);
void updatePatient(vector<Patient> &PATIENTS);
void updateDurationOfStay(Patient &patient);

// Function to read data and return a new vector
vector<Patient> readFileIntoVector()
{
    // Create an object out of the fstream class.
    fstream file;

    // Open file in binary and input mode.
    file.open(FILENAME, ios::binary | ios::in);

    // Temporary patient that we can use to update vector's data.
    Patient temporary_patient;

    // New vector with the new data
    vector<Patient> PATIENTS;

    // Read data into the vector
    while (file.read(reinterpret_cast<char*>(&temporary_patient), sizeof(temporary_patient)))
    {
        PATIENTS.push_back(temporary_patient);
    }

    // Close file
    file.close();

    // Return updated vector
    return PATIENTS;
}

// Vector cannot be a constant since we're using reinterpret_cast.
// A function that writes the updated vector to the file to override the data in the old file.
void writeVectorIntoFile(vector<Patient> &PATIENTS)
{
    // Create an object out of the fstream class.
    fstream file;

    // Open file in binary and output mode.
    // Note that there's no append mode, since we're overriding the file.
    file.open(FILENAME, ios::binary | ios::out);

    // For each patient in that vector
    for (auto &patient: PATIENTS)
    {
        // Write the current patient to the new updated file.
        file.write(reinterpret_cast<char*>(&patient), sizeof(patient));
    }

    // Close file
    file.close();
}

// A function that displays only a single patient
void displayPatient(Patient &patient)
{
    cout << endl << "==========================" << endl;
    cout << "Id: " << patient.id << endl;
    cout << "Name: " << patient.name << endl;

    if (patient.roomNumber == -1)
        cout << "Room is not assigned.";
    else
    {
        cout << "Room number: " << patient.roomNumber << endl;
        cout << "Room price =  $" << roomPrice << endl;

        if (patient.durationOfStayInDays == -1)
        {
            cout << "Duration of stay and Bill not calculated yet.";
        }
        else
        {
            // Calculate bill made the display patient function not a const.
            calculateBill(patient);
            cout << "Bill = " << patient.bill << endl;

            cout << "Duration of stay: " << patient.durationOfStayInDays << " days";
        }
        
    }
    cout << endl << "==========================" << endl;
}

// A function that displays the patients inside of a vector.
void displayPatients(vector<Patient> &PATIENTS)
{
    // For-Each loop to iterate and display each patient.
    for(auto &patient: PATIENTS)
    {
        displayPatient(patient);
    }
}

// A function that adds a patient to a vector.
void addPatient(vector<Patient> &PATIENTS)
{
    Patient patient;

    // Auto-increment patient's id
    if (PATIENTS.empty())
    {
        patient.id = 1;
    }
    else
    {
        // last patient's id in vector + 1
        patient.id = PATIENTS.back().id + 1;
    }
    

    // Input name
    string name;
    do
    {
        cout << "Enter name: ";
        getline(cin, name);

        if (name.length() > maxNameLength - 1)
            cout << "Name is too long. Max length is " << (maxNameLength - 1) << " characters." << endl;
    }
    while(name.length() > maxNameLength - 1); // Save one for NULL character

    /* Converting string to character array,
    Note that patient name's terminating character will
    always be NULL, but rest of character array may be
    filled with garbage values. */
    strcpy(patient.name, name.c_str()); 

    // Add patient to the vector.
    PATIENTS.push_back(patient);

    cout << endl << "Patient added succesfully.";
}

// A function that calculates which number a patient gets.
int calculatePatientId(vector<Patient> &PATIENTS)
{
    // The current patient id is basically the vector's size + 1
    return PATIENTS.size() + 1;
}

// A function that searches for a patient in a vector.
void searchForPatient(vector<Patient> &PATIENTS)
{
    // Input id from user.
    int id;
    cout << "Enter id of patient: "; cin >> id;

    // Input name from user.
    string name;
    cout << "Enter name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    // Iterate over vector to find patient.
    for(auto &patient: PATIENTS)
    {
        // Id and name match
        if(patient.id == id && patient.name == name)
        {
            // If found, then display the patient!
            cout << "Patient found!" << endl;
            displayPatient(patient);

            // Then return from the function, no need to continue.
            return;
        }
    }

    // If we reached here, then it means patient was not found...
    cout << "Patient was not found..." << endl;
}

void dischargePatient(vector<Patient> &PATIENTS)
{
    // Input id from user.
    int id;
    cout << "Enter id of the patient you wish to discharge: ";
    cin >> id;

    // Input name from user.
    string name;
    cout << "Enter name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    // Get vector size in a variable
    int vectorSize = PATIENTS.size();

    // For loop to keep track of the index
    for(int i = 0; i < vectorSize; i++)
    {   
        // Condition to match id and name
        if (id == PATIENTS[i].id && name == PATIENTS[i].name)
        {
            // Erase/Discharge patient using his index from the vector.
            PATIENTS.erase(PATIENTS.begin() + i);

            // Tell user discharging was successful
            cout << "Patient discharged successfully.";
            return;
        }
    }

    // If we reach over here, then it means we didn't find the patient.
    cout << "Patient does not exist." << endl;
}

// A Function that assigns a room to a patient.
void assignRoom(Patient &patient, vector<Patient> &PATIENTS)
{
    // Input roomNumber from user.
    int roomNumber;
    cout << "Enter room number you wish to have: ";
    cin >> roomNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Check if roomNumber is already taken by another patient.
    if (checkRoomNumber(roomNumber, PATIENTS))
    {
        cout << "Room is already assigned..." << endl;
        return;
    }
    else
    {
        // Else assign it and tell user it was succesful.
        patient.roomNumber = roomNumber;
        cout << "Room assigned succesfully." << endl;
    }
}

// A function that checks if a roomNumber is already assigned.
bool checkRoomNumber(int roomNumber, const vector<Patient> &PATIENTS)
{
    // Iterate over all patients in the vector
    for(const auto &patient: PATIENTS)
    {
        if (roomNumber == patient.roomNumber)
            return true; // True indicates that it's taken
    }

    // Otherwise return false
    return false;
}

void calculateBill(Patient &patient)
{
    patient.bill = roomPrice * patient.durationOfStayInDays;
}

void updatePatient(vector<Patient> &PATIENTS)
{
    // Input id from user.
    int id;
    cout << "Enter id of patient: "; cin >> id;

    // Input name from user.
    string name;
    cout << "Enter name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    // Keep track of patient index to update.
    int patientIndex;
    // Boolean to check if patient was found.
    bool foundPatient = false;

    // Get vector size.
    int vectorSize = PATIENTS.size();

    for(int i = 0; i < vectorSize; i++)
    {
        if(id == PATIENTS[i].id && name == PATIENTS[i].name)
        {
            foundPatient = true;
            patientIndex = i;
            break;
        }
    }

    if (foundPatient)
    {
        cout << "Patient found!" << endl;
        cout << "What do you wish to update?" << endl;
        cout << "1.Update name - 2.Assign Room - 3.Update duration of stay (in days)" << endl;

        int updatePatientProcessNumber;
        cout << "Enter updating process number: "; cin >> updatePatientProcessNumber;

        // Incase user wants to update name.
        string name;

        switch(updatePatientProcessNumber)
        {
            case 1:
                // Input name
                do
                {
                    cout << "Enter name: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, name);

                    if (name.length() > maxNameLength - 1)
                        cout << "Name is too long. Max length is " << (maxNameLength - 1) << " characters." << endl;
                }
                while(name.length() > maxNameLength - 1);
                strcpy(PATIENTS[patientIndex].name, name.c_str()); 
                break;
            case 2:
                assignRoom(PATIENTS[patientIndex], PATIENTS);
                break;
            case 3:
                updateDurationOfStay(PATIENTS[patientIndex]);
                break;
        }
    }
    else
    {
        cout << "Patient not found.." << endl;
    }
}

void updateDurationOfStay(Patient &patient)
{
    int duration;
    cout << "Enter new duration of stay: ";
    cin >> duration;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(duration < 0)
        cout << "Duration of stay cannot be less than 0..." << endl;
    else
        patient.durationOfStayInDays = duration;
}