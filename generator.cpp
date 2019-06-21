#include <iostream>
#include <vector>
#include <string> 
#include <stdlib.h>  
#include <fstream>
#include <map>
#include <string.h>

struct objects{
    std::string name;
    int ram, proc, solver_name;
};

void print_ascii_wordart() {

    std::cout <<"  ____ _____ _   _ _____ ____      _  _____ ___  ____ " <<std::endl;
    std::cout <<" / ___| ____| \\ | | ____|  _ \\    / \\|_   _/ _ \\|  _ \\ " <<std::endl;
    std::cout <<"| |  _|  _| |  \\| |  _| | |_) |  / _ \\ | || | | | |_) |" <<std::endl; 
    std::cout <<"| |_| | |___| |\\  | |___|  _ <  / ___ \\| || |_| |  _ < " <<std::endl; 
    std::cout <<" \\____|_____|_| \\_|_____|_| \\_\\/_/   \\_\\_| \\___/|_| \\_\\" <<std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


}

void map_to_formula(objects ** input, int N, int K) {
    std::ofstream formula ("formula.opb");
    int sum = 0;

    if(formula.is_open()) {
        // INITIAL LINE
        formula << "* #variable= " << N + N * K << " #constraint= " << 2 * (1 + N + K) << std::endl;

        // FUNCTION TO MINIMIZE
        formula << "min: ";
        
        for(size_t i = 0; i < N; i++) {
            formula << "+1 " << 'x' << input[0][i].solver_name << " ";
        }

        formula << ";" <<  std::endl;

        // RAM/PROC OF ALL HARDWARES MUST BE BIGGER THAN RAM/PROC OF ALL VMS

        formula << "* RAM hw > RAM vm" <<  std::endl;

        for(size_t i = 0; i < N; i++) {
            formula << "+" << input[0][i].ram << " x" << input[0][i].solver_name << ' ';
        }

        formula << ">= ";

        for(size_t i = 0; i < K; i++) {
            sum += input[1][i].ram;
    
        }

        formula << sum;

        formula <<  " ;" <<  std::endl;

        formula << "* PROC hw > PROC vm" <<  std::endl;

        for(size_t i = 0; i < N; i++) {
            formula << "+" << input[0][i].proc << " x" << input[0][i].solver_name << ' ';
            
        }

        formula << " >= ";

        sum = 0;

        for(size_t i = 0; i < K; i++) {
            sum += input[1][i].proc;
    
        }
        
        formula << sum;

        formula <<  " ;" <<  std::endl;

        // HARDWARE UPPER LIMIT (LIMIT OF VIRTUAL MACHINES THAT ARE WORKING ON A HARDWARE)
        sum = 0;
        formula << "* Upper limit RAM" <<  std::endl;

        for(size_t i = 0; i < N; i++) {
            for(size_t j = 0; j < K; j++) {
                formula << '+' << input[i + 1][j].ram << " ~x" << input[i + 1][j].solver_name << ' ';
            }

            formula << '+' << input[0][i].ram << " x" << input[0][i].solver_name << " >= ";
            
            for(size_t j = 0; j < K; j++) {
                sum +=  input[i + 1][j].ram;
            }
            formula << sum;
            sum = 0;

            formula <<  " ;" <<  std::endl;
        }

        sum = 0;
        formula << "* Upper limit PROC" <<  std::endl;


        for(size_t i = 0; i < N; i++) {
            for(size_t j = 0; j < K; j++) {
                formula << '+' << input[i + 1][j].proc << " ~x" << input[i + 1][j].solver_name << ' ';
            }

            formula << '+' << input[0][i].proc << " x" << input[0][i].solver_name << " >= ";
            
            for(size_t j = 0; j < K; j++) {
                sum +=  input[i + 1][j].proc;
            }
            formula << sum;
            sum = 0;

            formula <<  " ;" <<  std::endl;
        }

        // VM MUST BE ON
        formula << "* Pombos" <<  std::endl;
        for(size_t i = 0; i < K; i++) {
            for(size_t j = 0; j < N; j++) {
                formula << "+1 x" << input[j + 1][i].solver_name << ' ';
            }

            formula << ">= " << 1 <<  " ;" <<  std::endl;
        }

        // VM MUST BE ON AT ONLY ONE HARDWARE
        for(size_t i = 0; i < K; i++) {
            for(size_t j = 0; j < N; j++) {
                formula << "+1 ~x" << input[j + 1][i].solver_name << ' ';
            }

            formula << ">= " << N - 1 <<  " ;" <<  std::endl;
        }

    }

    formula.close();
}

void map_to_answer(std::vector<std::pair<int, int>> result, objects ** input, int N, int K) {
    int a = 0;
    std::cout << std::endl << "RESULT" << std::endl << std::endl;
    for (size_t i = 0; i <= N; i++) {
        for (size_t j = 0; j < K; j++) {
            if(input[i][j].solver_name == result[a].first && result[a].second == 1) { std::cout <<  input[i][j].name << " must be on;" << std::endl; a++; }
            else if(input[i][j].solver_name == result[a].first && result[a].second == 0) { std::cout <<  input[i][j].name << " must be off;" << std::endl; a++; }
        }
    }
    
}


int main(int argc, char * argv[]) {

    objects ** input;
    objects vms[100];
    objects hardware, vm; 
    int N, K, ram, proc, last = 0;

    print_ascii_wordart();

    std::cout << "Enter the amount of Hardware: ";
    std::cin >> N;

    input = (objects**) malloc(100 * sizeof(objects*));

    for(size_t i = 0; i < 100; i++) {
        input[i] = (objects *) calloc(100, sizeof(objects));
    }

    for(size_t i = 0; i < N; i++) {
        std::cout << "Enter your hardware" << i + 1 << " name, ram and processor: ";
        std::cin >> hardware.name >> hardware.ram >> hardware.proc;
        hardware.solver_name = (i + 1);
        last++;
        input[0][i] = hardware;
    }

    std::cout << std::endl << "Enter the amount of VM's: ";
    std::cin >> K;

    for(size_t i = 0; i < K; i++) {
            std::cout << "Enter your vm" << i + 1 << " name, ram and processor: ";           
            std::cin >> vm.name >> vm.ram >> vm.proc;
            vms[i] = vm;
    }



    for(size_t i = 1; i <= N; i++) {
        for(size_t j = 0; j < K; j++) { 
            input[i][j] = vms[j];
            input[i][j].solver_name = last + 1;
            last++;
            input[i][j].name += " on hardware " + input[0][i - 1].name;
        }
    }

    std::cout << std::endl;

    for(size_t i = 1; i <= N; i++) {
        for(size_t j = 0; j < K; j++) {
            std::cout << input[i][j].name << " is x" << input[i][j].solver_name << std::endl; 
        }
    }
    
    if(strcmp(argv[1], "-i") == 0) {
        map_to_formula(input, N, K);
    }

    else if(strcmp(argv[1], "-o") == 0) {
        std::ifstream myfile;
        myfile.open(argv[2]);
        std::vector<std::string> vec;
        std::vector<std::pair<int, int>> result;
        std::string answer;

        std::cin.ignore();
        while(myfile >> answer) {
            vec.push_back(answer);
        }

        for(size_t i = 0; i < vec.size(); i++) {
            
            if(vec[i][0] == 'x') {
                std::string str = vec[i].substr(1);
                result.push_back(std::pair<int, int>(stoi(str), 1));
                //std::cout << str;
            }

            else {
                std::string str = vec[i].substr(2);
                result.push_back(std::pair<int, int>(stoi(str), 0));
            }

        }

        //for(size_t i = 0; i < result.size(); i++) std::cout << result[i].second << std::endl; 

        map_to_answer(result, input, N, K);
    }

    for(size_t i = 0; i < 100; i++) {
            free(input[i]);
    }

    free(input);
    
    return 0;
}