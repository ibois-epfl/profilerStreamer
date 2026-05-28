#include "Utils.hh"

namespace ProfilerStreaming::Utils
{
    std::pair<double, double> linearRegressionSlope(const std::vector<ProfilerStreaming::SpatialData::PointCloudWithTimestamp>& data, int referenceIndex) 
    {
        size_t n = data.size();
        if (n < 2) return {0.0, 0.0};
        std::chrono::high_resolution_clock::time_point t0 = data.at(referenceIndex).GetTimestamp();
        double sum_t = 0, sum_x = 0, sum_tt = 0, sum_tx = 0;
        for (const auto& d : data) 
        {
            double t = std::chrono::duration_cast<std::chrono::microseconds>(d.GetTimestamp() - t0).count();
            double x = d.GetPoints().at(0).x();
            sum_t += t;
            sum_x += x;
            sum_tt += t * t;
            sum_tx += t * x;
        }
        double denom = n * sum_tt - sum_t * sum_t;
        if (denom == 0) return {0.0, 0.0};

        double a = (n * sum_tx - sum_t * sum_x) / denom;
        double b = (sum_x - a * sum_t) / n;
        return {a, b};
    } 

    Eigen::Matrix4d ComputeTransformationMatrix(const std::vector<Eigen::Vector3d>& sourcePoints, const std::vector<Eigen::Vector3d>& targetPoints)
    {
        if (sourcePoints.size() != targetPoints.size() || sourcePoints.empty()) 
        {
            throw std::runtime_error("Source and target points must be of the same size and not empty.");
        }

        Eigen::MatrixXd sourceMatrix(sourcePoints.size(), 3);
        Eigen::MatrixXd targetMatrix(sourcePoints.size(), 3);

        for (size_t i = 0; i < sourcePoints.size(); ++i) 
        {
            sourceMatrix.row(i) = sourcePoints[i];
            targetMatrix.row(i) = targetPoints[i];
        }

        Eigen::Matrix4d transformation = Eigen::umeyama(sourceMatrix.transpose(), targetMatrix.transpose(), false);

        return transformation;
    }
}