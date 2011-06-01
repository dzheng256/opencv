/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "test_precomp.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class CV_FFmpegWriteBigImagesTest : public cvtest::BaseTest
{
	public:
		void run(int)
		{
			try
			{
				Mat img = imread(string(ts->get_data_path()) + "readwrite/read.png", 0);
				if (img.empty()) ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_TEST_DATA);
				imwrite(string(ts->get_data_path()) + "readwrite/write.png", img);
			}
			catch(...)
			{
				ts->set_failed_test_info(cvtest::TS::FAIL_EXCEPTION);
			}
			ts->set_failed_test_info(cvtest::TS::OK);
		}
};

class CV_FFmpegWriteBigVideoTest : public cvtest::BaseTest
{
	public:
		void run(int)
		{
			const int img_r = 4096;
			const int img_c = 4096;
			Size frame_s = Size(img_c, img_r);
			const double fps = 30;
			const double time_sec = 1;

			Mat img(img_r, img_c, CV_8UC3, Scalar::all(0));
			try
			{
				VideoWriter writer(string(ts->get_data_path()) + "video/output.avi",
					CV_FOURCC('X', 'V', 'I', 'D'), fps, frame_s);

				if (writer.isOpened() == false) ts->set_failed_test_info(cvtest::TS::FAIL_EXCEPTION);

				for (int i = 0 ; i < static_cast<int>(fps * time_sec); i++ )
				{
					circle(img, Point2i(img_c / 2, img_r / 2), cv::min(img_r, img_c) / 2 * (i + 1), Scalar::all(255));
					writer << img;
				}
			}
			catch(...)
			{
				ts->set_failed_test_info(cvtest::TS::FAIL_EXCEPTION);
			}
			ts->set_failed_test_info(cvtest::TS::OK);
		}
};


TEST(Highgui_FFmpeg_WriteBigImage, regression) { CV_FFmpegWriteBigImagesTest test; test.safe_run(); }
TEST(Highgui_FFmpeg_WriteBigVideo, regression) { CV_FFmpegWriteBigVideoTest  test; test.safe_run(); }