void System::SaveKeyFrameTrajectoryTUM(const string &filename)
{
    cout << endl
         << "Saving keyframe trajectory to " << filename << " ..." << endl;

    {
        vector<KeyFrame *> vpKFs = mpMap->GetAllKeyFrames();
        sort(vpKFs.begin(), vpKFs.end(), KeyFrame::lId);

        // Transform all keyframes so that the first keyframe is at the origin.
        // After a loop closure the first keyframe might not be at the origin.
        //cv::Mat Two = vpKFs[0]->GetPoseInverse();

        ofstream f;
        f.open(filename.c_str());
        f << fixed;

        for (size_t i = 0; i < vpKFs.size(); i++)
        {
            KeyFrame *pKF = vpKFs[i];

            // pKF->SetPose(pKF->GetPose()*Two);

            if (pKF->isBad())
                continue;

            cv::Mat R = pKF->GetRotation().t();
            vector<float> q = Converter::toQuaternion(R);
            cv::Mat t = pKF->GetCameraCenter();
            f << setprecision(6) << pKF->mTimeStamp << setprecision(7) << " " << t.at<float>(0) << " " << t.at<float>(1) << " " << t.at<float>(2)
              << " " << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << endl;
        }

        f.close();
        cout << endl
             << "trajectory saved!" << endl;
    }

    cout << endl
         << "Saving point cloud" << endl;

    {
        ofstream f;
        f.open("./pointcloud.ply");
        f << fixed;

        std::vector<ORB_SLAM2::MapPoint *> points = mpMap->GetAllMapPoints();

        // header
        f << "ply" << endl
          << "format ascii 1.0" << endl
          << "element vertex " << points.size() << endl
          << "property float x" << endl
          << "property float y" << endl
          << "property float z" << endl
          << "end_header" << endl;

        for (size_t i = 0; i < points.size(); ++i)
        {
            cv::Mat coords = points[i]->GetWorldPos();
            float x = coords.at<float>(0);
            float y = coords.at<float>(1);
            float z = coords.at<float>(2);
            f << x << " " << y << " " << z << " " << endl;
        }

        f.close();
        cout << endl
             << "pointcloud saved!" << endl;
    }
}