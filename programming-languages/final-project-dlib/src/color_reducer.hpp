#ifndef COLOR_REDUCER_HPP_
#define COLOR_REDUCER_HPP_

#include <dlib/image_io.h>
#include <dlib/matrix.h>

#include <string>

namespace color_reducer
{

class ColorReducer
{
public:
    enum struct ImageType
    {
        JPEG,
        PNG,
        BMP,
    };

    // CONSTRUCTORS

    /* filename                  - file name with image to be filtered
     * clusters_number           - number of colors in filtered image
     * iterations_number         - number of iterations in kmeans algorithm
     * need_pick_initial_centers - if true, before clustering reasonable starting points will be found by
     *                             trying to find a set of points that are all far away from each other;
     *                             if false, random values for starting points will be used
     * need_resize_before_filter - if true, resizes image to have approximately area_after_resize pixels
     *                             which can make kmeans algorithm work faster; if resized image is going
     *                             to be bigger than the original one, no resize will take place;
     *                             if false, the original image will be used to find clusters
     * area_after_resize         - sets the number of pixels in resized image; if resize_before_filter is false,
     *                             this parameter has no meaning */
    ColorReducer(std::string filename = "",
                 unsigned clusters_number = 2,
                 unsigned iterations_number = 10,
                 bool need_pick_initial_centers = true,
                 bool need_resize_before_filter = true,
                 unsigned area_after_resize = 10000);

    // LOGICS

    /* Finds centers of clusters using kmeans algorithm */
    void fit();

    /* Redraws _image using colors that are in _centers */
    void filter();

    /* Fits, then filters */
    void fit_filter();

    /* Saves _filtered_image to a file
     * std::string - file name
     * ImageType - image type (jpeg, png or bmp) */
    void save_result(const std::string&, ImageType = ImageType::JPEG) const;

    // GETTERS

    const std::string& filename() { return m_filename; }
    unsigned clusters_number() { return m_clusters_number; }
    unsigned iterations_number() { return m_iterations_number; }
    bool need_pick_initial_centers() { return m_need_pick_initial_centers; }
    bool need_resize_before_filter() { return m_need_resize_before_filter; }
    bool is_initialized() { return m_is_initialized; }
    bool is_fitted() { return m_is_fitted; }
    bool is_filtered() { return m_is_filtered; }
    unsigned area_after_resize() { return m_area_after_resize; }
    const std::vector<dlib::matrix<int,3,1>>& centers() { return m_centers; }

    // SETTERS

    void filename(std::string);
    void clusters_number(unsigned);
    void iterations_number(unsigned);
    void need_pick_initial_centers(bool);
    void need_resize_before_filter(bool);
    void area_after_resize(unsigned);

private:
    std::string m_filename;

    std::vector<dlib::matrix<int,3,1>> m_centers;
    std::vector<dlib::matrix<int,3,1>> m_pixels;

    dlib::array2d<dlib::rgb_pixel> m_image;
    dlib::array2d<dlib::rgb_pixel> m_filtered_image;

    unsigned m_clusters_number;
    unsigned m_iterations_number;
    unsigned m_area_after_resize;

    bool m_need_pick_initial_centers;
    bool m_need_resize_before_filter;
    bool m_is_initialized = false;
    bool m_is_fitted = false;
    bool m_is_filtered = false;

    /* Loads image, resizes it to have _area_after_resize pixels if _resize_before_filter is true
     * Then puts pixels to _pixels, which will be used in kmeans clustering algorithm */
    void init_image();
};

}  // namespace color_reducer

#endif  // COLOR_REDUCER_HPP_
