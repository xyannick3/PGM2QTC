/**
 * @file filtrage.c
 * @author xyannick3 and CarapaceRose
 * @brief source file of the filtrage module.
 */


#include "../include/filtrage.h"
#include "../include/QuadTree.h"
#include <math.h>
#include <stdio.h>

/**
 * @brief calculate the variance of a specific node.
 * @param node the node that's studied.
 * @param tree the quadtree to have further info on the quadtree.
 * @return variance
 */

double computeVariance(QuadTreeNode* node, QuadTree* tree){
    if (node->leaf || node->u)
        return 0;
    int index = node - tree->nodes;
    unsigned char m1 = tree->nodes[index * 4 + 1].m;
    unsigned char m2 = tree->nodes[index * 4 + 2].m;
    unsigned char m3 = tree->nodes[index * 4 + 3].m;
    unsigned char m4 = tree->nodes[index * 4 + 4].m;

    double mean = (m1 + m2 + m3 + m4) / 4.0;
    double mu = pow(m1,2) + pow(mean - m1, 2) +
                pow(m2, 2) + pow(mean - m2, 2) +
                pow(m3, 2) + pow(mean - m3, 2) +
                pow(m4, 2) + pow(mean - m4, 2);
    double variance = sqrt(mu) /4.0;

    return variance;
}

/**
 * @brief computer the medvar and maxvar of a quadtree.
 * @param tree the quadtree.
 * @param medvar the calculated medvar
 * @param maxvar the maximal variance calculated.
 */
void computeVars(QuadTree *tree, double* medvar, double* maxvar){
    double totalVariance = 0.0;
    double maxVariance = 0.0;
    int varianceCount = 0;

    for (int i = 0; i < tree->totalNodes; i++){
        QuadTreeNode* node = &tree->nodes[i];

        if (node->leaf || node->u){
            continue;
        }
        double variance = computeVariance(node, tree);
        if (variance > 0.0) {
            totalVariance += variance;
            if (variance > maxVariance) {
                maxVariance = variance;
            }
            varianceCount++;
        }
    }
    if (varianceCount > 0){
        *medvar = totalVariance / varianceCount;
    }else {
        *medvar = 0.0;
    }
    *maxvar = maxVariance;
}
/**
 * @brief filtrageNode is a recursive function that filters the nodes of the tree.
 * @param tree the quadtree.
 * @param nodeIndex the index of the studied node.
 * @param sigma the calculated sigma.
 * @param alpha the alpha.
 */
int filtrageNode(QuadTree* tree, int nodeIndex, double sigma, double alpha){
    QuadTreeNode* node = &tree->nodes[nodeIndex];

    if (node->u || node->leaf)
        return 1;
    
    int uniformCount = 0;
    for (int i = 1; i <= 4; i++){
        int childIndex = nodeIndex * 4 + i;
        uniformCount += filtrageNode(tree, childIndex, sigma * alpha, alpha);
    }
    if ((uniformCount < 4) || (computeVariance(node, tree) > sigma))
        return 0;
    node->epsilon = 0;
    node->u = 1;
    return 1;
}
/**
 * @brief function that filter the nodes of the quadtree 
 * @param tree the quadtree that's to be filtered.
 * @param alpha the value that determines how much the quadtree is to be filtered.
 */
void filtrage(QuadTree* tree, double alpha){
    double medvar = 0.0, maxvar = 0.0;
    computeVars(tree, &medvar, &maxvar);
    printf("medvar %lf | maxvar %lf\n", medvar, maxvar);
    double sigma = medvar / maxvar;

    filtrageNode(tree, 0, sigma, alpha);
}

