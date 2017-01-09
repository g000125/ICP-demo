#include"function.h"

pcl::PointCloud<PointT>
obj2cloud(string obj_filename){
	vtkSmartPointer<vtkPolyData> polydata;
	vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
	//vtkSmartPointer<vtkDataReader> reader2 = vtkSmartPointer<vtkDataReader>::New();
	reader->SetFileName(obj_filename.c_str());
	reader->Update();
	polydata = reader->GetOutput();
	pcl::PointCloud<PointT> cloud;
	vtkPolyDataToPointCloud(polydata, cloud);
	return cloud;
}

void
showCloud(pcl::PointCloud<PointT> cloud,string windows_name){
	pcl::PointCloud<PointT>::Ptr cloud_pr(new pcl::PointCloud<PointT>);
	*cloud_pr = cloud;

	pcl::visualization::PCLVisualizer viewer(windows_name);
	viewer.setCameraPosition(0, 0, -3.0, 0, -1, 0);

	//ori is	
//	viewer.addCoordinateSystem(0.3);
	viewer.addCoordinateSystem(0.3,"reference",0);

	viewer.addPointCloud(cloud_pr);

	while (!viewer.wasStopped())
		viewer.spinOnce(100);
	
}



void 
addRgb(pcl::PointCloud<PointT>::Ptr cloud_pr, int rgb){
	for (size_t i = 0; i < cloud_pr->size(); ++i){
		cloud_pr->points[i].rgb = rgb;
		}
}
