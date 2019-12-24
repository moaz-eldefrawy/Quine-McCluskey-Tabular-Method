#include <bits/stdc++.h>

using namespace std;

/*
4
10
0 1 2 5 6 7 8 9 10 14



1 3 5 7 8 10 13 15
*/
unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

vector<int> inputs;
int n,num_of_varaibles;
map< pair< int, vector<int> > , bool > is_left_prime_implicant;
vector< pair<int, vector<int> > >  pre_table[100];
vector < pair<int, vector<int> > > after_table[100];
vector < pair<int, vector<int> > > left_prime_implicants;
vector < pair<int, vector<int> > > emptie[100];
vector < vector<int> > extended_prime_implicants;
bool chosen_implicants[1000];
int index;
vector<int> current_vector;
int a;
vector<int> b;

string expression(int i){
    int x = left_prime_implicants[i].first;
    vector <int> y = left_prime_implicants[i].second;
    string s = bitset< 64 >( x ).to_string();
    s = s.substr( s.size()-num_of_varaibles, s.size() );

  //  cout << s << endl;
    for(int i=0; i<y.size(); i++){
        s[ s.size() - log2(y[i]) - 1 ]='9';
    }
   // cout << s << endl;
    string output = "";
    for(int i=0; i<s.size(); i++){
        if( s[i] == '0' ){
            output +=  char('A' + i);
            output +=  "~";
        } else if(s[i] == '1'){
            output +=  char('A' + i);
        }
    }
    return output;

}

vector <int> min_exp;
int min_cost = 10000;

bool cover_all(vector<int> r){
    for(int i=0; i<inputs.size(); i++){
        int w = inputs[i];
        bool exist = false;
        for(int j=0; j<r.size(); j++){
            for(int k=0; k<extended_prime_implicants[r[j]].size(); k++){
                if(w == extended_prime_implicants[r[j]][k])
                    exist = true;
            }
        }

        if(exist == false)
            return false;
    }
    return true;
}


void find_minimum_implicants(int i, vector<int> r){
  //  cout << i << endl;
    if(i == extended_prime_implicants.size()){
        if(cover_all(r) == false) return;

        int cost = 0;
      //  cout << "ok \n";
        for(int j=0; j<r.size(); j++){
            cost += num_of_varaibles - left_prime_implicants[r[j]].second.size();
        }
        if(cost < min_cost){
            min_cost = cost;
            min_exp = r;
        }
        return ;
    }

    find_minimum_implicants(i+1,r);
    r.push_back(i);
    find_minimum_implicants(i+1,r);
    r.pop_back();
    return;

}

bool match_and_minimize(){
    bool minimization = false;
    // reset all prime implicants to be left
    for(int i=0; i<4; i++){
        for(int j=0; j<pre_table[i].size(); j++){
            is_left_prime_implicant[pre_table[i][j]] = true;
        }

    }



    for(int i=0; i<99; i++){
        for(int j1=0; j1<pre_table[i].size(); j1++){
            for(int j2=0; j2<pre_table[i+1].size(); j2++){
                //int a =  table[i].first;
                pair< int, vector<int> > elm1 = pre_table[i][j1];
                pair< int, vector<int> > elm2 = pre_table[i+1][j2];
               // cout << countSetBits(elm1.first ^ elm2.first) <<" "<< elm1.first  <<" - " << elm2.first << endl;
                if( countSetBits(elm1.first ^ elm2.first) == 1 && elm1.second == elm2.second  ) // works
                {
                    minimization = true;
                    is_left_prime_implicant[elm1] = false;
                    is_left_prime_implicant[elm2] = false;
                    pair<int, vector<int> > p;
                    p.first = elm1.first;
                    p.second = elm1.second;
                    p.second.push_back(elm2.first-elm1.first);
                    sort(p.second.begin(), p.second.end());
                    after_table[i].push_back(p);
                }
            }
        }
    }
/*

*/
    for(int i=0; i<4; i++){
        for(int j=0; j<after_table[i].size(); j++){
            cout << after_table[i][j].first <<" - ";
            vector <int> elms = after_table[i][j].second;
            for(int k=0; k<elms.size(); k++)
                cout << elms[k] <<" ";

            cout << endl;
        }

        cout << endl;
    }


    // get all left prime implicants
    cout << " Left Prime implicants\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<pre_table[i].size(); j++){
            if(is_left_prime_implicant[pre_table[i][j]] == true){
                left_prime_implicants.push_back(pre_table[i][j]);
                cout << after_table[i][j].first <<" - ";
                vector <int> elms = after_table[i][j].second;
                for(int k=0; k<elms.size(); k++)
                    cout << elms[k] <<" ";

                cout << endl;
            }
        }
    }
    return minimization;
}

void get_all_possible_values(int i, int sum){
    if(i == b.size()){
        current_vector.push_back(sum+a);
        return ;
    }
    get_all_possible_values(i+1, sum);
    get_all_possible_values(i+1, sum+b[i]);
    return;
}

void extend_prime_implicants(){
    for(int i=0; i<left_prime_implicants.size(); i++){
        a = left_prime_implicants[i].first;
        b = left_prime_implicants[i].second;
        index = i;
        current_vector.clear();
        get_all_possible_values(0,0);
        extended_prime_implicants.push_back(current_vector);

     //   extended_prime_implicants.assign()
    }
}

void clear_extra_prime_implicants(){
    for(int i=0; i<left_prime_implicants.size(); i++){
        for(int j=i+1; j<left_prime_implicants.size(); j++){
            if(left_prime_implicants[i] == left_prime_implicants[j]){
                left_prime_implicants.erase(left_prime_implicants.begin() + j);
            }
        }
    }
}



void define_essential_prime_implicants(){
    bool is_essential;
    for(int i=0; i<extended_prime_implicants.size(); i++){

        for(int j=0; j<extended_prime_implicants[i].size(); j++){
            is_essential = true;
            for(int i1=0; i1<extended_prime_implicants.size(); i1++){
                if(i == i1) continue;
                for(int j1=0; j1<extended_prime_implicants[i1].size(); j1++){
                    if(extended_prime_implicants[i][j] == extended_prime_implicants[i1][j1]){
                        is_essential = false;
                        break;
                    }
                }
                if(is_essential == false)
                    break;
            }
            if(is_essential == true) // a prime implicant found
                break;

        }
        if(is_essential){
            chosen_implicants[i] = 1;
        }

    }

}

int main()
{
    memset(chosen_implicants,false,sizeof(chosen_implicants));
    cin >> num_of_varaibles;
    cin >> n;//
    while(n--){
        int a;
        cin >> a;
        inputs.push_back(a);
    }
    n = inputs.size();

    for(int i=0; i<n; i++){
        int numOfBits = countSetBits(inputs[i]);
        vector<int> empt;
        int a = inputs[i];
        pair<int, vector<int> > p =
        make_pair(a,empt);
        pre_table[numOfBits].push_back(p);

    }

     cout << "Groups \n";
    for(int i=0; i<4; i++){
        for(int j=0; j<pre_table[i].size(); j++)
            cout << pre_table[i][j].first <<" ";

        cout << endl;
    }
    while(true){
        bool Minimization_done = match_and_minimize();
        if(Minimization_done == false)
            break;
        for(int i=0; i<50; i++){
            pre_table[i] = after_table[i];
            after_table[i].clear();
        }
    }

    cout << "ALL PRIME IMPLICANTS\n";
    for(int i=0; i<left_prime_implicants.size(); i++){
        cout << left_prime_implicants[i].first <<" >> ";
        for(int k=0; k<left_prime_implicants[i].second.size(); k++){
            cout << left_prime_implicants[i].second[k] <<" ";
        }
        cout << endl;
    }

    clear_extra_prime_implicants();
    cout << "AFTER REMOVING DUPLICATED PRIME IMPLICANTS:\n\n";
    cout << "Number Of Prime Implicants is: \n\n" << left_prime_implicants.size() << endl;
    for(int i=0; i<left_prime_implicants.size(); i++){
        cout << expression(i) <<" --------- ";
        cout << left_prime_implicants[i].first <<" >> ";
        for(int k=0; k<left_prime_implicants[i].second.size(); k++){
            cout << left_prime_implicants[i].second[k] <<" ";
        }
        cout << endl;
    }


    extend_prime_implicants();
    cout << "PRIME IMPLICANTS AFTER EXTENSION\n";

    for(int i=0; i<extended_prime_implicants.size(); i++){
        for(int j=0; j<extended_prime_implicants[i].size(); j++)
            cout << extended_prime_implicants[i][j] <<" ";

        cout << endl;
    }

    define_essential_prime_implicants();
//    for(int i=0; i<ex)
    cout << "Essential Prime Implicants: \n";
    for(int i=0; i<extended_prime_implicants.size(); i++){
        if(chosen_implicants[i] == 1){
            cout << expression(i) << " ---- " ;
            for(int j=0; j<extended_prime_implicants[i].size(); j++)
                cout << extended_prime_implicants[i][j] <<" ";

            cout << endl;
        }
    }
    cout << endl;

    vector<int> dum;
    find_minimum_implicants(0,dum);
    cout << "f = ";
    for(int i=0; i<min_exp.size(); i++){
        cout << expression(min_exp[i]);
        if(i != min_exp.size() - 1)
            cout << " + " ;
    }
    cout << endl;



}
