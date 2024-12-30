#include <iostream.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <fstream.h>
#include <string.h>

const int MAX        = 120;
const int MAX_ITER   = 1000;
const double EPSILON = 0.0001;

struct Point {
    double x;
    double y;
};

void initializeMembershipMatrix(double membership[][MAX], int numData, int numClusters) {
    for (int i = 0; i < numData; i++) {
        for (int j = 0; j < numClusters; j++) {
            membership[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

double calculateDistance(Point p1, Point p2) {
    double distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    return distance;
}

void fuzzyCMeansClustering(Point data[], int numData, int numClusters, double fuzziness) {
    double membership[MAX][MAX];
    Point centroids[MAX];
    double prevObjective = 0.0;
    double currObjective = 0.0;

    initializeMembershipMatrix(membership, numData, numClusters);

    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Update centroids
        for (int j = 0; j < numClusters; j++) {
            double sumX = 0.0;
            double sumY = 0.0;
            double sumMembership = 0.0;

            for (int i = 0; i < numData; i++) {
                double membershipPower = pow(membership[i][j], fuzziness);
                sumX += membershipPower * data[i].x;
                sumY += membershipPower * data[i].y;
                sumMembership += membershipPower;
            }

            centroids[j].x = sumX / sumMembership;
            centroids[j].y = sumY / sumMembership;
        }

        // Menghitung Fungsi Objektif
        currObjective = 0.0;
        for (int i = 0; i < numData; i++) {
            for (int j = 0; j < numClusters; j++) {
                double distance = calculateDistance(data[i], centroids[j]);
                currObjective += pow(membership[i][j], fuzziness) * pow(distance, 2);
            }
        }

        // Update membership matrix
        for (int i = 0; i < numData; i++) {
            for (int j = 0; j < numClusters; j++) {
                double numerator = calculateDistance(data[i], centroids[j]);
                double denominatorSum = 0.0;

                for (int k = 0; k < numClusters; k++) {
                    double denominator = calculateDistance(data[i], centroids[k]);
                    denominatorSum += pow(numerator / denominator, 2.0 / (fuzziness - 1));
                }

                membership[i][j] = 1.0 / denominatorSum;
            }
        }

        // Cek Pemberhenti
        if (abs(currObjective - prevObjective) < EPSILON) {
            break;
        }

        prevObjective = currObjective;
    }

    // Output cluster
    for (int i = 0; i < numData; i++) {
        int maxCluster = 0;
        double maxMembership = 0.0;

        for (int j = 0; j < numClusters; j++) {
            if (membership[i][j] > maxMembership) {
                maxCluster = j;
                maxMembership = membership[i][j];
            }
        }

        cout << "Data ke-" << i+1 << " tergabung ke dalam cluster : " << maxCluster+1 << endl;
    }
}

int main() {
    // Open file notepad (input data)
    ifstream inputFile("data.txt");
    if (!inputFile) {
        cout << "Gagal membuka file input!" << endl;
        return 1;
    }

    Point data[MAX];
    int numData = 0;
    char line[100];

    while (inputFile.getline(line, sizeof(line))) {
        char *token = strtok(line, ",");
        data[numData].x = atof(token);
        token = strtok(NULL, ",");
        data[numData].y = atof(token);
        numData++;
    }

    inputFile.close();

    // Parameter clustering
    int numClusters;
    double fuzziness;

    cout<<"CLUSTERING DATA BMKG DENGAN ALGORITMA FUZZY C-MEANS\n"<<endl;
	 cout<<"Kelompok 1"<<endl;
    cout<<" 1. Vina Amaylia K. (082011233044)"<<endl;
    cout<<" 2. Akrom Fuadi     (082011233079)\n"<<endl;

    cout<<"Input nilai-nilai yang dibutuhkan dalam algoritma FCM"<<endl;
    cout<<" >> Masukkan jumlah cluster yang diinginkan : ";cin>>numClusters;
    cout<<" >> Masukkan nilai pangkat yang diingikan   : ";cin>>fuzziness;
	 cout<<"\nAkan didapatkan output sebagai berikut,\n"<<endl;

    fuzzyCMeansClustering(data, numData, numClusters, fuzziness);

    getch();
    return 0;
}
