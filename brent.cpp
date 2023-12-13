#include "brent.h"
#include <iostream>
using namespace std;

Brent::Brent(int table_size){
        data_vec.resize(table_size);
}
int Brent::hash(int key) const{
        return key % data_vec.size();
}
int Brent::hash2(int key) const{
        return int(key/data_vec.size())%data_vec.size();
    }
vector<int> Brent::calculateLQ(int index, int step) {
        vector<int> temp;
        temp.push_back(index);
        while (data_vec[index].valid == true) {
            // Collision: Linear quotient
            index = (index + step)%data_vec.size();
            temp.push_back(index);
        }
     
        return temp;
}
void Brent::insert(int key){
    int index = hash(key);
    int step = hash2(key);
    
    int S_count = 1;
    int i_s, j_s = 1;
    
    vector<int> lq = calculateLQ(index, step);
    
    for(int i = 0; i < lq.size(); i++){
        if(!data_vec[lq[i]].valid){
            if(S_count >= 3){
                for(int s = 0; s < lq.size(); s++){
                    int key_temp = data_vec[lq[s]].data;
                    int index_temp = hash(key_temp);
                    int step_temp = hash2(key_temp);
                    
                    vector<int> bq = calculateLQ(index_temp, step_temp);
                    
                    for(int x = 0; x < bq.size(); x++){
                        if(!data_vec[bq[x]].valid){
                            if(i_s + j_s < S_count){
                                data_vec[bq[x]].data = key_temp;
                                data_vec[bq[x]].valid = true;
                                
                                data_vec[lq[s]].data = key;
                                return;
                            }
                        }
                        else{
                            j_s++;
                            bq = calculateLQ(index_temp, step_temp);
                        }
                    }
                i_s++;
                j_s = 1;
                }
                data_vec[lq[i]].data = key;
                data_vec[lq[i]].valid = true;
                return;
                 
            }
            else{
                data_vec[lq[i]].data = key;
                data_vec[lq[i]].valid = true;
                return;
            }
        }
        else{
            lq = calculateLQ(index, step);
            S_count += 1;
        }
    }

}



int Brent::find_num_probes(int key) const{
    int index = hash(key);
    int step = hash2(key);

    int count = 1;

    while (data_vec[index].data != key) {
        index = (index + step) % data_vec.size();
        count++;
    }

    return count;
}



double Brent::find_average_num_probes() const{
    double probe_sum = 0;
    int entries = 0;

    for (const entry &e : data_vec) {
        if (e.valid) {
            probe_sum += find_num_probes(e.data);
            entries++;
        }
    }

    return (probe_sum / entries);
}