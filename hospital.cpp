#include "hospitalFunctions.cpp"

int main()
{
    // Vector for patients data.
    vector<Patient> PATIENTS;

    // Process number for menu drive.
    int processNumber;

    // Condition to ask user
    char userContinue = 'y';

    while(tolower(userContinue) == 'y')
    {
        // Read file into vector
        PATIENTS = readFileIntoVector();
        
        // Menu drive:
        cout << endl << "===================================" << endl;
        cout << "1. Display Patients" << endl;
        cout << "2. Add Patient" << endl;
        cout << "3. Search for Patient" << endl;
        cout << "4. Update Patient" << endl;
        cout << "5. Discharge Patient";
        cout << endl << "===================================" << endl;
    
        // Input user for process number
        cout << "Enter process number: ";
        cin >> processNumber;

        // To clean up newline character left in the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(processNumber)
        {
            // Don't forget the breaks!
            case 1:
                // Display patients
                displayPatients(PATIENTS); break;
            case 2:
                // Add patients
                addPatient(PATIENTS);
                // Write vector to file // Update the file
                writeVectorIntoFile(PATIENTS); break;
            case 3:
                // Search for patient
                searchForPatient(PATIENTS); break;
            case 4:
                // Update patient
                updatePatient(PATIENTS);
                // Write vector to file // Update the file
                writeVectorIntoFile(PATIENTS); break;
            case 5:
                // Discharge patient
                dischargePatient(PATIENTS);
                // Write vector to file // Update the file
                writeVectorIntoFile(PATIENTS); break;
        }

        cout << endl << "Do you wish to continue(y/n): ";
        cin >> userContinue;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}