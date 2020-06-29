#include "color_reducer.hpp"
#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, char* argv[]) try
{
    using color_reducer::color_reducer;
    namespace po = boost::program_options;

    std::string ifile, ofile, output_file_type;
    unsigned int n_clusters, n_iterations, area_after_resize;
    bool pick_initial_centers, resize_before_filter;
    po::options_description desc("Allowed options");
    desc.add_options()
            // First parameter describes option name/short name
            // The second is parameter to option
            // The third is description
            ("help,H", "Print this message")
            ("input,I", po::value<std::string>()->required(), "Input filename")
            ("output,O", po::value<std::string>()->required(), "Output filename")
            ("output_file_type,T", po::value<std::string>()->required(), "Output file type (etiher png, jpeg (jpg) or bmp)")
            ("n_clusters,N", po::value<unsigned int>()->required(), "Number of colors in output image")
            ("n_iterations", po::value<unsigned int>()->default_value(100), "Number of iterations in clustering algorithm")
            ("resize_before_filter", po::bool_switch()->default_value(true),
             "Resize image to area_after_resize pixels to increase perfomance")
            ("area_after_resize", po::value<unsigned int>()->default_value(10000), "Number of pixels in image after resize")
            ("pick_initial_centers", po::bool_switch()->default_value(true),
             "Try to find a set of points that are all far away from each other")
            ;

    po::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 0;
    }
    ifile = vm["input"].as<std::string>();
    ofile = vm["output"].as<std::string>();
    n_clusters = vm["n_clusters"].as<unsigned int>();
    n_iterations = vm["n_iterations"].as<unsigned int>();
    pick_initial_centers = vm["pick_initial_centers"].as<bool>();
    resize_before_filter = vm["resize_before_filter"].as<bool>();
    area_after_resize = vm["area_after_resize"].as<unsigned int>();
    output_file_type = vm["output_file_type"].as<std::string>();
    if (output_file_type != "png" && output_file_type != "jpg" && output_file_type != "jpeg" && output_file_type != "bmp")
    {
        std::cerr << "Output file type must be etiher png, jpeg (jpg) or bmp\n";
        return 1;
    }
    color_reducer filter(ifile, n_clusters, n_iterations, pick_initial_centers, resize_before_filter, area_after_resize);
    filter.fit_filter();
    if (output_file_type == "png")
        filter.save_result(ofile, color_reducer::image_type::png);
    else if (output_file_type == "bmp")
        filter.save_result(ofile, color_reducer::image_type::bmp);
    else
        filter.save_result(ofile, color_reducer::image_type::jpeg);
}
catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
}

