#ifndef COLOR_REDUCER_HPP
#define COLOR_REDUCER_HPP

#include <dlib/image_io.h>
#include <dlib/matrix.h>
#include <string>

namespace color_reducer
{
    class color_reducer
    {        
        std::string _file;
        unsigned int _n_clusters, _n_iterations;
        bool _pick_initial_centers, _resize_before_filter, _is_initialized = false, _is_fitted = false, _is_filtered = false;
        unsigned int _area_after_resize;
        dlib::array2d<dlib::rgb_pixel> _image, _filtered_image;
        std::vector<dlib::matrix<int,3,1> > _centers, _pixels;

        /* Loads image, resizes it to have _area_after_resize pixels if _resize_before_filter is true
         * Then puts pixels to _pixels, which will be used in kmeans clustering algorithm */
        void init_image();

        public:
        enum struct image_type
        {
            jpeg, png, bmp,
        };

        //CONSTRUCTORS

        /* f                    - file name with image to be filtered
         * n_clusters           - number of colors in filtered image
         * n_iterations         - number of iterations in kmeans algorithm
         * pick_initial_centers - if true, before clustering reasonable starting points will be found by
         *                        trying to find a set of points that are all far away from each other;
         *                        if false, random values for starting points will be used
         * resize_before_filter - if true, resizes image to have approximately area_after_resize pixels
         *                        which can make kmeans algorithm work faster; if resized image is going
         *                        to be bigger than the original one, no resize will take place;
         *                        if false, the original image will be used to find clusters
         * area_after_resize    - sets the number of pixels in resized image; if resize_before_filter is false,
         *                        this parameter has no meaning */
        color_reducer(std::string f = "", unsigned int n_clusters = 2, unsigned int n_iterations = 10, bool pick_initial_centers = true,
                      bool resize_before_filter = true, unsigned int area_after_resize = 10000);

        //LOGICS

        /* Finds centers of clusters using kmeans algorithm */
        void fit();

        /* Redraws _image using colors that are in _centers */
        void filter();

        /* Fits, then filters */
        void fit_filter();

        /* Saves _filtered_image to a file
         * std::string - file name
         * image_type - image type (jpeg, png or bmp)*/
        void save_result(const std::string &, image_type = image_type::jpeg) const;

        //GETTERS
        std::string file() {return _file; }
        unsigned int n_clusters() { return _n_clusters; }
        unsigned int n_iterations() { return _n_iterations; }
        bool pick_initial_centers() { return _pick_initial_centers; }
        bool resize_before_filter() { return _resize_before_filter; }
        bool is_initialized() { return _is_initialized; }
        bool is_fitted() { return _is_fitted; }
        bool is_filtered() { return _is_filtered; }
        unsigned int area_after_resize() { return _area_after_resize; }
        std::vector<dlib::matrix<int,3,1> > centers() { return _centers; }

        //SETTERS
        void file(std::string);
        void n_clusters(unsigned int);
        void n_iterations(unsigned int);
        void pick_initial_centers(bool);
        void resize_before_filter(bool);
        void area_after_resize(unsigned int);
    };
}

#endif // COLOR_REDUCER_HPP
