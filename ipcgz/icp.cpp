//gaozheng 1601214430
#define	_CRT_SECURE_NO_WARNINGS
#include"function.h"
#include<pcl/registration/icp.h>
#include<pcl/filters/voxel_grid.h>

pcl::PointCloud<PointT> final_cloud;
pcl::PointCloud<PointT> tmp_cloud;
pcl::PointCloud<PointT>::Ptr cloud_pr(new pcl::PointCloud<PointT>);
pcl::PointCloud<PointT>::Ptr cloud_dest_pr(new pcl::PointCloud<PointT>);
pcl::IterativeClosestPoint<PointT, PointT> icp;

int filte(PointCloud<PointT>::Ptr src_cloud,pcl::PointCloud<PointT> dest_cloud){
	pcl::VoxelGrid<PointT> sor;				//创建滤波对象
	sor.setInputCloud(src_cloud);           //设置需要过滤的点云给滤波对象
	//sor.setLeafSize(0.1f,0.1f,0.1f);      //设置滤波时创建的体素大小为1cm立方体
	sor.filter(dest_cloud);                 //执行滤波处理，存储输出cloud_filtered
	return 0;
}

int regist(pcl::PointCloud<PointT> src_cloud,pcl::PointCloud<PointT> dest_cloud){
	//icp实现
	*cloud_pr = src_cloud;
	*cloud_dest_pr = dest_cloud;
	icp.setInputSource(cloud_pr);  //源点云
	icp.setInputTarget(cloud_dest_pr);//目标点云
	//icp 参数设置
    icp.setMaximumIterations(100);			//最大迭代次数。
	icp.setEuclideanFitnessEpsilon(0.1);	//相邻两次迭代方差的差值约束。
	icp.setTransformationEpsilon(1e-10);	//上次转换与当前转换的差值，这个值一般设为1e-6或者1e-10或者更小，可以用来终止迭代次数。
	icp.setMaxCorrespondenceDistance(0.7);	//忽略在此距离之外的点（默认距离单位是m）。
	//执行配准后并将结果保存到final_cloud中
	icp.align(final_cloud);
	//icp匹配后的转换矩阵及欧式适合度评分，其中欧式适合度评分是计算输出点云到最近目标点云对应点对的距离平方和。
	cout << "has converged: " << icp.hasConverged() << endl
		 << "score: " << icp.getFitnessScore() << endl;
	cout << icp.getFinalTransformation() << endl;  
	return 0;
}

int main(){
	//读入obj数据
	pcl::PointCloud<PointT> cloud1 = obj2cloud("1.obj");
	pcl::PointCloud<PointT> cloud2 = obj2cloud("2.obj");
	pcl::PointCloud<PointT> cloud3 = obj2cloud("3.obj");
	pcl::PointCloud<PointT> cloud4 = obj2cloud("4.obj");
	pcl::PointCloud<PointT> cloud5 = obj2cloud("5.obj");
	pcl::PointCloud<PointT> cloud6 = obj2cloud("6.obj");
	pcl::PointCloud<PointT> cloud7 = obj2cloud("7.obj");
	pcl::PointCloud<PointT> cloud8 = obj2cloud("8.obj");
	tmp_cloud=cloud1;//初始化，设置目标点云

	//writer 用于保存文件，使用方式 writer.write("result.pcd", final_cloud, false);
	pcl::PCDWriter writer;
	//记录四元数变换（仅定义，未使用）
	Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity (), prev, targetToSource;

	regist(cloud2,cloud1);
	tmp_cloud+=final_cloud;
	regist(cloud3,final_cloud);
	tmp_cloud+=final_cloud;
	regist(cloud4,final_cloud);
	tmp_cloud+=final_cloud;
	regist(cloud5,final_cloud);
	tmp_cloud+=final_cloud;
	regist(cloud6,final_cloud);
	tmp_cloud+=final_cloud;
	regist(cloud7,final_cloud);
	tmp_cloud+=final_cloud;
	regist(cloud8,final_cloud);
	tmp_cloud+=final_cloud;

	writer.write("resultttt.pcd", tmp_cloud, false);
	//pcl::PointCloud<PointT> filted_cloud;
	//*cloud_pr=tmp_cloud;
	//filte(cloud_pr,filted_cloud);
	//writer.write("resulfilted.pcd", filted_cloud, false);
	for (size_t i = 0; i < tmp_cloud.size(); ++i){
		tmp_cloud.points[i].rgb =9.1476764e-041 ;//绿
		//tmp_cloud.points[i].rgb =3.5733111e-043 ;//蓝
		//tmp_cloud.points[i].rgb = 2.3418052e-038;  //红
	}
	showCloud(tmp_cloud, "final_cloud");
	cout<< "ICP finish" << endl;
	system("pause");
	return (0);
}
