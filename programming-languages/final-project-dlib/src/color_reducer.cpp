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

// CONSTRUCTORS

ColorReducer::ColorReducer(std::string filename,
                           unsigned clusters_number,
                           unsigned iterations_number,
                           bool need_pick_initial_centers,
                           bool need_resize_before_filter,
                           unsigned area_after_resize)
    : m_filename(std::move(filename))
    , m_clusters_number(clusters_number)
    , m_iterations_number(iterations_number)
    , m_area_after_resize(area_after_resize)
    , m_need_pick_initial_centers(need_pick_initial_centers)
    , m_need_resize_before_filter(need_resize_before_filter)
{
}

// SETTERS

void ColorReducer::filename(std::string value)
{
    m_filename = std::move(value);
    m_is_initialized = false;
}

void ColorReducer::need_resize_before_filter(bool value)
{
    m_need_resize_before_filter = value;
    m_is_initialized = false;
}

void ColorReducer::area_after_resize(unsigned value)
{
    m_area_after_resize = value;
    m_is_initialized = false;
}

void ColorReducer::clusters_number(unsigned value)
{
    m_clusters_number = value;
    m_is_fitted = false;
}

void ColorReducer::iterations_number(unsigned value)
{
    m_iterations_number = value;
    m_is_fitted = false;
}

void ColorReducer::need_pick_initial_centers(bool value)
{
    m_need_pick_initial_centers = value;
    m_is_fitted = false;
}

// LOGICS

void ColorReducer::init_image()
{
    try
    {
        dlib::load_image(m_image, m_filename);
        if (m_need_resize_before_filter)
        {
            dlib::array2d<dlib::rgb_pixel> resized_image;
            double mul = std::sqrt(m_image.size() / m_area_after_resize);
            if (mul > 1)
                resized_image.set_size(static_cast<long>(m_image.nr() / mul), static_cast<long>(m_image.nc() / mul)); // NOLINT google-runtime-int
            else
                resized_image.set_size(m_image.nr(), m_image.nc());
            dlib::resize_image(m_image, resized_image);
            std::size_t cur_pixel = 0;
            m_pixels.resize(resized_image.size());
            for (const auto& pixel : resized_image)
                m_pixels[cur_pixel++] = { pixel.red, pixel.green, pixel.blue };
        }
        else
        {
            std::size_t cur_pixel = 0;
            m_pixels.resize(m_image.size());
            for (const auto & pixel : m_image)
                m_pixels[cur_pixel++] = { pixel.red, pixel.green, pixel.blue };
        }
        m_is_initialized = true;
    }
    catch (const dlib::image_load_error& e)
    {
        m_is_initialized = false;
        m_is_fitted = false;
        m_image.clear();
        m_filtered_image.clear();
        m_pixels.clear();
        m_centers.clear();
        m_filename.clear();
        throw e;
    }
}

void ColorReducer::fit()
{
    if (!m_is_initialized)
        init_image();

    if (m_is_fitted)
        return;

    if (m_need_pick_initial_centers)
    {
        dlib::pick_initial_centers(m_clusters_number, m_centers, m_pixels);
    }
    else
    {
        m_centers.resize(m_clusters_number);
        std::mt19937 prng (std::random_device{}());
        std::uniform_int_distribution<int> dist(1, 255);
        for (std::size_t i = 0; i < m_clusters_number; ++i)
            m_centers[i] = { dist(prng), dist(prng), dist(prng) };
    }

    dlib::find_clusters_using_kmeans(m_pixels, m_centers, m_iterations_number);
    m_is_fitted = true;
}

void ColorReducer::filter()
{
    m_filtered_image.set_size(m_image.nr(), m_image.nc());
    dlib::matrix<int, 3, 1> m_temp;
    for (long i = 0; i < m_image.nr(); ++i)  // NOLINT google-runtime-int
    {
        for (long j = 0; j < m_image[i].nc(); ++j)  // NOLINT google-runtime-int
        {
            m_temp = m_centers[dlib::nearest_center(m_centers, dlib::matrix<int,3,1>{ m_image[i][j].red,
                                                                                      m_image[i][j].green,
                                                                                      m_image[i][j].blue, })];
            m_filtered_image[i][j] =
            {
                static_cast<unsigned char>(m_temp(0)),
                static_cast<unsigned char>(m_temp(1)),
                static_cast<unsigned char>(m_temp(2)),
            };
        }
    }
    m_is_filtered = true;
}

void ColorReducer::fit_filter()
{
    fit();
    filter();
}

void ColorReducer::save_result(const std::string& filename, ColorReducer::ImageType format) const
{
    try
    {
        switch (format)
        {
        case ImageType::JPEG:
        {
            dlib::save_jpeg(m_filtered_image, filename);
            break;
        }
        case ImageType::PNG:
        {
            dlib::save_png(m_filtered_image, filename);
            break;
        }
        case ImageType::BMP:
        {
            dlib::save_bmp(m_filtered_image, filename);
            break;
        }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

}  // namespace color_reducer
