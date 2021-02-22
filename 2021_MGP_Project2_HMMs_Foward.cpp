/* Assigment 2: HMMs Forward
 * Description: Implement the Foward Algorithm for estimating the probability of a sequence of observations give the model.
 * The program should work for any discrete HMM an any observation sequence.
 *
 * Version: 1.0
 * Date: 2021 February
 * By: Mario De Los Santos
 * Instituto Nacional de Astrofísica Óptica y Electónica.
 *
 * Class: Probabilistic Graphical Models
 * By: PhD Enrique Sucar
 *
 * Reference; Sucar, L. E. (2015). Probabilistic graphical models.
 *            Advances in Computer Vision and Pattern Recognition.
 *            London: Springer London. doi, 10(978), 2
 */
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//CSV Class to extract the information
//General use to export

class File_handling{
    string file_2extract;
//To add version v2.0, you should be able to pass the information thru a .txt or .csv file
};

//Class for HMMs Fowarding
class HMMs_Forward{
    /*Predefined limit for observations and states
    Those,px and py, will define the maximum size for the matrixes*/
#define px 10
#define py 10

    //Observation and states numbers should be defined as first step
    int No_states;
    int No_Observations;

    /*Variables that would take the information needed calculate the fowarding and direct method
     * π =Prior Probability vector, A = Transistion Matrix, B = Observation Matrix
     * To do: fit the vectors with the csv data. */
    float Prob_vector[px] = {0}; //π
    /*Prior prob vector structure:
     *
     * π = S0,S1,S2, ..., S10
     * The number of accepted states can be improved by modifying px variable,
     * right now delimited to 10 possibles states;
     *
     * */
    float Trans_matrix[px][py]={0}; //A
    /* Transition Matrix, representes the cost of the transition between states,
     * the matrix structure following the next staments
     *        S0 S1 S2 S3 ... Sn
     * A = S0 .  .  .
     *     S1 .  .  .
     *     S2 .  .  .
     *     S3 .  .  .
     *     .  .  .  .
     *     .  .  .  .
     *     .  .  .  .
     *     Sn .  .  .
     * */
    float Observation_matix[px][py]={0};//B
    /* Observation matrix represent the probability for each state to reach determinated "status",
     * the matrix structure following the next staments
     *        S0 S1 S2 S3 ... Sn
     * B = O0 .  .  .
     *     O1 .  .  .
     *     O2 .  .  .
     *     O3 .  .  .
     *     .  .  .  .
     *     .  .  .  .
     *     .  .  .  .
     *     On .  .  .
     * */

    //Array that would recoled the desired sequence by the user, the size could be smaller.
    int sequence[25]={0};

    // This could be a local variable if we need to have to improve the memory management
    double alpha[100];
public:
    //Parameterized constructor
    HMMs_Forward(int N_Sts, int N_Obs)
    //The user should pass the number of states and observations that the problem has
    {
        if(N_Sts|| N_Obs != 0) { //Verification for a valid information
            No_Observations = N_Obs; //Data hiding, information will be part of the object class
            No_states = N_Sts; //Will keep private

            //Functions to extract the matrixes with the needed information
            Probability_vector(No_states);
            Transition_matrix(No_Observations);
            Obs_matrix(No_states, No_Observations);
        }
        else
            cout<<"Invalid register"<<endl;

    }
    /*The function will recolect the Probaility vector needed to operate the foward funtion:
     * Even considering we should read this from the csvm this should work as the first approach
     * The probability of each state is going to be positioned with the next structure:
     *          State 1, State 2, State 3, State 4, State N
     *  A[100]= {0 , 1 , 2 , 3 , 4 , ... , 100 }
     **/
private:
    void Probability_vector(int n_states)
    {
        cout<<"Insert Probability Vector: "<<endl;
        for(int i=0;i<n_states;i++)
        {
            cout<<"S"<<i<<endl;
            cin>>Prob_vector[i];}
        // Debbuging process
        cout<<"Confirmation"<<endl;
        for(int i=0;i<n_states;i++) {
            cout<<"S"<<i<<" "<<Prob_vector[i]<<" ";
        }
    }
    void Obs_matrix(int n_states, int n_obs)
    {
        cout<<"Insert Observation Matrix: "<<endl;
        for(int i=0;i<n_states;i++) {
            for (int j = 0; j < n_obs; j++) {
                cout << "S" << i << " to O" << j << endl;
                cin >> Observation_matix[i][j];
            }
        }
        // Debbuging process
        for(int i=0;i<n_states;i++)
        {
            for(int j=0;j<n_obs;j++) {
                cout <<Observation_matix[i][j]<<" ";
            }cout<<endl;}
    }
    void Transition_matrix(int n_states)
    {
        cout<<"Insert Transition Matrix: "<<endl;
        for(int i=0;i<n_states;i++){
            for(int j=0;j<n_states;j++) {
                cout << "S" << i << " to S" << j << endl;
                cin >> Trans_matrix[i][j];
            }
        }
        for(int i=0;i<n_states;i++){
            for(int j=0;j<n_states;j++) {
                cout<< Trans_matrix[i][j]<<" ";
            }cout<<endl;
        }
    }

public:
    //Once configured the constructor, is possible to call the function to operate the iterative method
    //The user should pase the lengh of the
    double Fordward_Operation(int sequence_size)
    {
        double aux=0;
        double sum=0;

        //Backip matrix just to keep alpha clean of the operations until the last step in the inductive step
        double alpha_backup[100]={0};

        //Sequence definition
        cout<<"Define sequence in numerical values"<<endl;
        for(int i=0;i<sequence_size;i++)
            cin>>sequence[i];
        //Inicialization
        for(int i=0;i<No_states;i++){
            alpha[i]=Prob_vector[i]*Observation_matix[i][sequence[0]];
        }
        //Induction
        //Moving around the sequence size given by the user
        for(int t=1;t<sequence_size;t++)
        {
            //Moving thru the states
            for(int j=0;j<No_states;j++) {
                aux=0; //Reset the sum each cicle from the += operations
                //Moving thru the states
                for (int k = 0; k<No_states; k++) {
                    //Summation of both last states multiplicated for the transition cost
                    aux += alpha[k] * Trans_matrix[k][j];
                }
                //save the value in the array multiplication of the aux * the observation probability of the related state
                alpha_backup[j] = aux * Observation_matix[j][sequence[t]];
                /*Take care, the sequence[t] indicates the probability of observation referenced to the given sequence
                 *for the user
                 *Example: consider for the basic coin example HHTT, we request it in binary form, so 0=H and T=1
                 *Then they would be referenced in a 1D array,
                 *            H  H   T   T
                 * Sequence = 0, 0 , 1 , 1
                 *
                 * Then considering the position, in the for loop of states, we would access the needed observations
                 *
                 *  Consider the next observation matrix
                 *          |M1=0  |M2=1
                 *  B = H=0 |0.8   |0.2
                 *      T=1 |0.2   |0.8
                 *
                 * So, if the loop is in the state M2 which is 1 and the sequence is in T=1 then the value taken from
                 * the last matrix would be P(T|M2) = 0.8.
                 * */
            }
            //The array alpha shall be changing wich eeach iteration, taking the calculeted values,
            //Basically this is the main goal of the algorithm.
            for(int i=0;i<No_states;i++)
            {
                alpha[i] = alpha_backup[i];
            }
        }
        //Last step is to sum the branches of the array, then return the value
        for(int i=0;i<No_states;i++) {
            sum += alpha[i];
        }
        return sum;
    }
};
int main()
{
    double result = 0; //Variable just to reproduce the return of the class

    //Object creation, passing the predefined parameters, then you would need to fit the vectors needed to operate
    HMMs_Forward T1(3,3);

    //Operational functions will return the Probability of the given sequence that is going to be requested here.
    result = T1.Fordward_Operation(4);

    //Basic stuff
    cout<<"Resultado: "<<result<<endl;

}

