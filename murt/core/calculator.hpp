#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <set>
#include <utility>
#include <math.h>
#include <vector>

#include "vec3.hpp"

#define LIGHT_SPEED 299792458.0f
#define AIR_IOR 1.00029f

static float GetDelay(std::vector<Vec3> points, float speed)
{
    float total_distance = 0.0f;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        total_distance += Vec3::Distance(points[i], points[i + 1]);
    }
    return total_distance / speed;
}

static float DirectPathLoss(Vec3 txPos, Vec3 rxPos, float txFreq)
{
    return 20.0f * log10(Vec3::Distance(txPos, rxPos)) + 20.0f * log10(txFreq) - 147.5f;
}

static float GetRefCoe(Vec3 txPos, Vec3 rxPos, Vec3 refPos, float matPerm, bool isTM)
{
    Vec3 refToTx = txPos - refPos;
    refToTx.Normalize();
    Vec3 refToRx = rxPos - refPos;
    refToRx.Normalize();

    // IOR - Index of Refraction
    // Relative permittivity according to ITU-R, P.2040-1. (for 1-100 GHz)
    constexpr float n1 = AIR_IOR; // Air IOR
    float n2 = matPerm;
    constexpr float c = LIGHT_SPEED;

    float c1 = c / sqrt(n1);
    float c2 = c / sqrt(n2);

    float angle_1 = Vec3::Angle(refToTx, refToRx) / 2.0f;
    float angle_2 = asin(c2 * sin(angle_1) / c1);

    float coe = 1.0f;

    if (sqrt(abs(n1 / n2)) * sin(angle_1) >= 1)
        return 1.0f;

    coe = (isTM) ? (sqrt(n2) * cos(angle_1) - sqrt(n1) * cos(angle_2)) /
                       (sqrt(n2) * cos(angle_1) + sqrt(n1) * cos(angle_2))
                 : (sqrt(n1) * cos(angle_1) - sqrt(n2) * cos(angle_2)) /
                       (sqrt(n1) * cos(angle_1) + sqrt(n2) * cos(angle_2));
    return coe;
}

static float ReflectedPathLoss(Vec3 txPos, Vec3 rxPos, Vec3 refPos, float txFreq, float matPerm)
{
    float ref_coe = GetRefCoe(txPos, rxPos, refPos, matPerm, true); // assuming all rays are TM
    float distance = Vec3::Distance(txPos, refPos) + Vec3::Distance(refPos, rxPos);
    return 20.0f * log10(distance) + 20.0f * log10(txFreq) - 20.0f * log10(abs(ref_coe)) - 147.55f;
}

static float GetVValue(Vec3 txPos, Vec3 rxPos, Vec3 edgePos, float txFreq)
{
    float r1 = Vec3::Distance(txPos, edgePos);
    float r2 = Vec3::Distance(edgePos, rxPos);
    float s1_s2 = Vec3::Distance(txPos, rxPos);
    Vec3 tx_to_rx_dir = (rxPos - txPos);
    tx_to_rx_dir.Normalize();
    Vec3 tx_to_edge_dir = (edgePos - txPos);
    tx_to_edge_dir.Normalize();
    float angle = Vec3::Angle(tx_to_edge_dir, tx_to_rx_dir);
    float h = r1 * sin(angle);
    float wave_length = LIGHT_SPEED / txFreq;
    return h * sqrt((2 * s1_s2) / (wave_length * r1 * r2));
}

static float GetCValue(float v)
{
    return 6.9 + 20.0 * log(sqrt(pow(v - 0.1, 2) + 1) + v - 0.1f);
}

static std::pair<float, float> GetCorrectionCorine(Vec3 txPos, Vec3 nearTxPos, Vec3 centerPos, Vec3 nearRxPos, Vec3 rxPos)
{
    txPos.y_ = 0.0f;
    nearTxPos.y_ = 0.0f;
    centerPos.y_ = 0.0f;
    nearTxPos.y_ = 0.0f;
    rxPos.y_ = 0.0f;

    float d1 = Vec3::Distance(txPos, nearTxPos);
    float d2 = Vec3::Distance(nearTxPos, centerPos);
    float d3 = Vec3::Distance(centerPos, nearRxPos);
    float d4 = Vec3::Distance(nearRxPos, rxPos);

    std::pair<float, float> cosines;
    cosines.first = sqrt((d1 * (d3 + d4)) / ((d1 + d2) * (d2 + d3 + d4)));
    cosines.second = sqrt((d4 * (d1 + d2)) / ((d3 + d4) * (d2 + d3 + d1)));

    return cosines;
}

static float DiffractedPathLoss(Vec3 txPos, Vec3 rxPos, std::vector<Vec3> edges, float txFreq)
{
    // Diffraction Loss is calculate according to the multiple knife edges by
    // Ensure the order of edges
    Vec3::ReorderEdges(txPos, edges);
    float pl = DirectPathLoss(txPos, rxPos, txFreq);
    if (edges.size() == 0)
        return pl;
    if (edges.size() == 1)
    {
        pl += GetCValue(GetVValue(txPos, rxPos, edges[0], txFreq));
        return pl;
    }

    if (edges.size() == 2)
    {
        Vec3 &nearTxEdge = edges[0];
        Vec3 &nearRxEdge = edges[1];
        float nearTxEdgeV = GetVValue(txPos, rxPos, nearTxEdge, txFreq);
        float nearRxEdgeV = GetVValue(txPos, rxPos, nearRxEdge, txFreq);
        float mainDiffractionLoss, supportDiffractionLoss;
        if (nearTxEdgeV > nearRxEdgeV)
        {
            mainDiffractionLoss = GetCValue(nearTxEdgeV);
            supportDiffractionLoss = GetCValue(GetVValue(nearTxEdge, rxPos, nearRxEdge, txFreq));
        }
        else
        {
            mainDiffractionLoss = GetCValue(nearRxEdgeV);
            supportDiffractionLoss = GetCValue(GetVValue(txPos, nearRxEdge, nearRxEdge, txFreq));
        }
        pl += mainDiffractionLoss + supportDiffractionLoss;
        return pl;
    }

    // triple edges
    Vec3 nearTxEdge, centerEdge, nearRxEdge;
    if (edges.size() == 3)
    {
        nearTxEdge = edges[0];
        centerEdge = edges[1];
        nearRxEdge = edges[2];
    }
    else
    {
        // pick up highest v edges
        std::set<std::pair<float, Vec3> > ordered_v;
        for (Vec3 &edge : edges)
        {
            ordered_v.insert({GetVValue(txPos, txPos, edge, txFreq), edge});
        }
        std::vector<Vec3> ordered_edges;
        for (auto &[v, edge] : ordered_v)
            ordered_edges.push_back(edge);
        std::vector<Vec3> top_edges;
        top_edges.push_back(ordered_edges[0]);
        top_edges.push_back(ordered_edges[1]);
        top_edges.push_back(ordered_edges[2]);

        Vec3::ReorderEdges(txPos, top_edges);
        nearTxEdge = top_edges[0];
        centerEdge = top_edges[1];
        nearRxEdge = top_edges[2];
    }

    float nearTxEdgeV = GetVValue(txPos, rxPos, nearTxEdge, txFreq);
    float centerEdgeV = GetVValue(txPos, rxPos, centerEdge, txFreq);
    float nearRxEdgeV = GetVValue(txPos, rxPos, nearRxEdge, txFreq);
    float maxEdgeV = std::max({nearTxEdgeV, centerEdgeV, nearRxEdgeV});
    float mainDiffractionLoss, supportDiffractionLoss1, supportDiffractionLoss2;

    if (nearTxEdgeV == maxEdgeV)
    {
        // near tx edge is main edge
        mainDiffractionLoss = GetCValue(nearTxEdgeV);
        supportDiffractionLoss1 = GetCValue(GetVValue(nearTxEdge, nearRxEdge, centerEdge, txFreq));
        supportDiffractionLoss2 = GetCValue(GetVValue(centerEdge, rxPos, nearRxEdge, txFreq));
    }
    else if (nearRxEdgeV == maxEdgeV)
    {
        // near rx edge is main edge
        mainDiffractionLoss = GetCValue(nearRxEdgeV);
        supportDiffractionLoss1 = GetCValue(GetVValue(txPos, centerEdge, nearTxEdge, txFreq));
        supportDiffractionLoss2 = GetCValue(GetVValue(nearTxEdge, nearRxEdge, centerEdge, txFreq));
    }
    else
    {
        // center edge is main edge
        mainDiffractionLoss = GetCValue(centerEdgeV);
        supportDiffractionLoss1 = GetCValue(GetVValue(txPos, centerEdge, nearTxEdge, txFreq));
        supportDiffractionLoss2 = GetCValue(GetVValue(centerEdge, rxPos, nearRxEdge, txFreq));
    }
    pl += mainDiffractionLoss + supportDiffractionLoss1 + supportDiffractionLoss2;
    return pl;
}
#endif