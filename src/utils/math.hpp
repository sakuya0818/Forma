#ifndef DANDELION_UTILS_MATH_HPP
#define DANDELION_UTILS_MATH_HPP

#include <cmath>
#include <tuple>

#include <Eigen/Core>

/*!
 * \ingroup utils
 * \file utils/math.hpp
 * \~chinese
 * \brief ����ļ��ṩһЩ����ʹ�õ���ѧ������
 */

const Eigen::Matrix3f I3f = Eigen::Matrix3f::Identity();
const Eigen::Matrix4f I4f = Eigen::Matrix4f::Identity();

/*!
 * \ingroup utils
 * \~chinese
 * \brief ���� float �� double ���͵� \f$\pi\f$ ֵ��
 */
template<typename T>
T pi();

template<>
inline constexpr double pi<double>()
{
    return 3.141592653589793238462643383279;
}

template<>
inline constexpr float pi<float>()
{
    return 3.14159265358f;
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ���Ƕ�ת��Ϊ���ȡ�
 */
template<typename T>
inline constexpr T radians(T degrees)
{
    return degrees / static_cast<T>(180.0) * pi<T>();
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ������ת��Ϊ�Ƕȡ�
 */
template<typename T>
inline constexpr T degrees(T radians)
{
    return radians / pi<T>() * static_cast<T>(180.0);
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ��һ������ƽ����
 */
template<typename T>
inline constexpr T squ(T x)
{
    return x * x;
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ��һ�����ض��ڸ��������½�֮��
 */
template<typename T>
inline constexpr T clamp(T low, T high, T value)
{
    return std::max(low, std::min(high, value));
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ��һ��\f$1\times 3\f$��������ת��Ϊ\f$1\times 4\f$�ķ�������
 */
template<typename T>
inline constexpr Eigen::Vector<T, 4> to_vec4(Eigen::Vector<T, 3> vec3)
{
    return Eigen::Vector<T, 4>(vec3.x(), vec3.y(), vec3.z(), static_cast<T>(0.0));
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ������ \f$\mathbf{I}\f$ �������� \f$\mathbf{N}\f$ �ķ��䡣
 * \param I ��ʾ�������ߵ������������ǵ�λ������
 * \param N ����ƽ��ķ��ߣ������ǵ�λ������
 */
inline Eigen::Vector3f reflect(const Eigen::Vector3f& I, const Eigen::Vector3f& N)
{
    return I - 2 * I.dot(N) * N;
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ���ź�������������ֵ���Ϊ 1������Ϊ -1�������ֵ����� 0.
 */
template<typename T>
inline constexpr T sign(T x)
{
    if (x > static_cast<T>(0.0)) {
        return static_cast<T>(1.0);
    }
    if (x < static_cast<T>(0.0)) {
        return static_cast<T>(-1.0);
    }
    return static_cast<T>(0.0);
}

/*!
 * \ingroup utils
 * \~chinese
 * \brief ����ת����Ԫ����ʾ��ʽת��Ϊ ZYX ŷ���Ǳ�ʾ��ʽ��
 *
 * ����������� ZYX ˳�棨������������ \f$z, y, x\f$ ����ת������ Dandelion
 * ������ϵԼ���£�ZYX ˳���Ӧ roll-yaw-pitch ��ת˳�򣨹�ת�����򡢸�������
 *
 * ����ŷ���ǹ��е�ȱ�ݣ����������������������ǣ���Ʒ���� yaw���� \f$\pm 90^\circ\f$
 * ����ʱ�������������Ͷ������⡣
 *
 * ��Ȼ�����ķ���ֵ�� `tuple`����ʹ�� `tuple` ���ܷ���ֵ�������㣬�����ýṹ����
 * ```cpp
 * auto [x_angle, y_angle, z_angle] = quaternion_to_ZYX_euler(w, x, y, z);
 * ```
 * �� `std::tie` �����ܷ���ֵ��
 *
 * \returns һ��Ԫ�飬�������������������� x �ᡢ�� y ����� z �����ת�ǡ�
 */
template<typename T>
inline std::tuple<T, T, T> quaternion_to_ZYX_euler(T w, T x, T y, T z)
{
    const T test = x * z + w * y;
    constexpr T threshold = static_cast<T>(0.5 - 1e-6);
    T x_rad, y_rad, z_rad;
    if (std::abs(test) > threshold) {
        x_rad = static_cast<T>(0.0);
        y_rad = sign(test) * pi<T>() / static_cast<T>(2.0);
        z_rad = sign(test) * static_cast<T>(2.0) * std::atan2(x, w);
    }
    else {
        x_rad =
            std::atan2(static_cast<T>(-2.0) * (y * z - w * x), squ(w) - squ(x) - squ(y) + squ(z));
        y_rad = std::asin(static_cast<T>(2.0) * (x * z + w * y));
        z_rad =
            std::atan2(static_cast<T>(-2.0) * (x * y - w * z), squ(w) + squ(x) - squ(y) - squ(z));
    }
    const T x_angle = degrees(x_rad);
    const T y_angle = degrees(y_rad);
    const T z_angle = degrees(z_rad);
    return std::make_tuple(x_angle, y_angle, z_angle);
}

#endif // DANDELION_UTILS_MATH_HPP
