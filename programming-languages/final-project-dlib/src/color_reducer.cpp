#include "color_reducer.hpp"
#include <dlib/clustering.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/matrix.h>
#include <random>
#include <string>
#include <utility>

namespace color_reducer
{
    //CONSTRUCTORS
    color_reducer::color_reducer(std::string f, unsigned int n_clusters, unsigned int n_iterations, bool pick_initial_centers,
                                 bool resize_before_filter, unsigned int area_after_resize) :
        _file(std::move(f)), _n_clusters(n_clusters), _n_iterations(n_iterations), _pick_initial_centers(pick_initial_centers),
        _resize_before_filter(resize_before_filter), _area_after_resize(area_after_resize), _image(0, 0), _filtered_image(0, 0), _centers(0), _pixels(0)
    {
    }

    //SETTERS
    void color_reducer::file(std::string file)
    {
        _file = std::move(file);
        _is_initialized = false;
    }
    void color_reducer::resize_before_filter(bool resize_before_filter)
    {
        _resize_before_filter = resize_before_filter;
        _is_initialized = false;
    }
    void color_reducer::area_after_resize(unsigned int area_after_resize)
    {
        _area_after_resize = area_after_resize;
        _is_initialized = false;
    }
    void color_reducer::n_clusters(unsigned int n_clusters)
    {
        _n_clusters = n_clusters;
        _is_fitted = false;
    }
    void color_reducer::n_iterations(unsigned int n_iterations)
    {
        _n_iterations = n_iterations;
        _is_fitted = false;
    }
    void color_reducer::pick_initial_centers(bool pick_initial_centers)
    {
        _pick_initial_centers = pick_initial_centers;
        _is_fitted = false;
    }

    //LOGICS
    void color_reducer::init_image()
    {
        try
        {
            dlib::load_image(_image, _file);
            if (_resize_before_filter)
            {
                dlib::array2d<dlib::rgb_pixel> _resized_image;
                double mul = std::sqrt(_image.size() / _area_after_resize);
                if (mul > 1)
                    _resized_image.set_size(static_cast<long>(_image.nr() / mul), static_cast<long>(_image.nc() / mul)); //NOLINT (google-runtime-int)
                else
                    _resized_image.set_size(_image.nr(), _image.nc());
                dlib::resize_image(_image, _resized_image);
                std::size_t cur_pixel = 0;
                _pixels.resize(_resized_image.size());
                for (auto & pixel : _resized_image)
                    _pixels[cur_pixel++] = {pixel.red, pixel.green, pixel.blue};
            }
            else
            {
                std::size_t cur_pixel = 0;
                _pixels.resize(_image.size());
                for (auto & pixel : _image)
                    _pixels[cur_pixel++] = {pixel.red, pixel.green, pixel.blue};
            }
            _is_initialized = true;
        }
        catch(const dlib::image_load_error& e)
        {
            _is_initialized = false;
            _is_fitted = false;
            _image.clear();
            _filtered_image.clear();
            _pixels.clear();
            _centers.clear();
            _file.clear();
            throw dlib::image_load_error{e};
        }
    }

    void color_reducer::fit()
    {
        if (!_is_initialized)
            init_image();
        if (_is_fitted)
            return;
        if(_pick_initial_centers)
            dlib::pick_initial_centers(_n_clusters, _centers, _pixels);
        else
        {
            _centers.resize(_n_clusters);
            std::mt19937 prng (std::random_device{}());
            std::uniform_int_distribution<int> dist(1, 255);
            for (std::size_t i = 0; i < _n_clusters; ++i)
                _centers[i] = {dist(prng), dist(prng), dist(prng)};
        }
        dlib::find_clusters_using_kmeans(_pixels, _centers, _n_iterations);
        _is_fitted = true;
    }

    void color_reducer::filter()
    {
        _filtered_image.set_size(_image.nr(), _image.nc());
        dlib::matrix<int, 3, 1> m_temp;
        for (long i = 0; i < _image.nr(); ++i) //NOLINT(google-runtime-int)
        {
            for (long j = 0; j < _image[i].nc(); ++j) //NOLINT(google-runtime-int)
            {
                m_temp = _centers[dlib::nearest_center(_centers, dlib::matrix<int,3,1> {_image[i][j].red, _image[i][j].green, _image[i][j].blue})];
                _filtered_image[i][j] = {static_cast<unsigned char>(m_temp(0)), static_cast<unsigned char>(m_temp(1)), static_cast<unsigned char>(m_temp(2))};
            }
        }
        _is_filtered = true;
    }

    void color_reducer::fit_filter()
    {
        fit();
        filter();
    }

    void color_reducer::save_result(const std::string & file, color_reducer::image_type format) const
    {
        try
        {
            switch (format)
            {
                case image_type::jpeg:
                {
                    dlib::save_jpeg(_filtered_image, file);
                    break;
                }
                case image_type::png:
                {
                    dlib::save_png(_filtered_image, file);
                    break;
                }
                case image_type::bmp:
                {
                    dlib::save_bmp(_filtered_image, file);
                    break;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}
